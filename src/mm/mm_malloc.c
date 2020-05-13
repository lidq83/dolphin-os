/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef NULL
#define NULL ((void *)0)
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_malloc
 *
 * Description:
 *  Find the smallest chunk that satisfies the request. Take the memory from
 *  that chunk, save the remaining, smaller chunk (if any).
 *
 *  8-byte alignment of the allocated data is assured.
 *
 ****************************************************************************/

FAR void *mm_malloc(FAR struct mm_heap_s *heap, size_t size)
{
	FAR struct mm_freenode_s *node;
	void *ret = NULL;
	int ndx;

	/* Handle bad sizes */

	if (size < 1)
	{
		return NULL;
	}

	/* Adjust the size to account for (1) the size of the allocated node and
   * (2) to make sure that it is an even multiple of our granule size.
   */

	size = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);

	/* We need to hold the MM semaphore while we muck with the nodelist. */

	mm_takesemaphore(heap);

	/* Get the location in the node list to start the search. Special case
   * really big allocations
   */

	if (size >= MM_MAX_CHUNK)
	{
		ndx = MM_NNODES - 1;
	}
	else
	{
		/* Convert the request size into a nodelist index */

		ndx = mm_size2ndx(size);
	}

	/* Search for a large enough chunk in the list of nodes. This list is
   * ordered by size, but will have occasional zero sized nodes as we visit
   * other mm_nodelist[] entries.
   */

	for (node = heap->mm_nodelist[ndx].flink;
		 node && node->size < size;
		 node = node->flink)
		;

	/* If we found a node with non-zero size, then this is one to use. Since
   * the list is ordered, we know that is must be best fitting chunk
   * available.
   */

	if (node)
	{
		FAR struct mm_freenode_s *remainder;
		FAR struct mm_freenode_s *next;
		size_t remaining;

		/* Remove the node.  There must be a predecessor, but there may not be
       * a successor node.
       */

		node->blink->flink = node->flink;
		if (node->flink)
		{
			node->flink->blink = node->blink;
		}

		/* Check if we have to split the free node into one of the allocated
       * size and another smaller freenode.  In some cases, the remaining
       * bytes can be smaller (they may be SIZEOF_MM_ALLOCNODE).  In that
       * case, we will just carry the few wasted bytes at the end of the
       * allocation.
       */

		remaining = node->size - size;
		if (remaining >= SIZEOF_MM_FREENODE)
		{
			/* Get a pointer to the next node in physical memory */

			next = (FAR struct mm_freenode_s *)(((FAR char *)node) + node->size);

			/* Create the remainder node */

			remainder = (FAR struct mm_freenode_s *)(((FAR char *)node) + size);
			remainder->size = remaining;
			remainder->preceding = size;

			/* Adjust the size of the node under consideration */

			node->size = size;

			/* Adjust the 'preceding' size of the (old) next node, preserving
           * the allocated flag.
           */

			next->preceding = remaining | (next->preceding & MM_ALLOC_BIT);

			/* Add the remainder back into the nodelist */

			mm_addfreechunk(heap, remainder);
		}

		/* Handle the case of an exact size match */

		node->preceding |= MM_ALLOC_BIT;
		ret = (void *)((FAR char *)node + SIZEOF_MM_ALLOCNODE);
	}

	mm_givesemaphore(heap);

	/* If CONFIG_DEBUG_MM is defined, then output the result of the allocation
   * to the SYSLOG.
   */

#ifdef CONFIG_DEBUG_MM
	if (!ret)
	{
		mwarn("WARNING: Allocation failed, size %d\n", size);
	}
	else
	{
		minfo("Allocated %p, size %d\n", ret, size);
	}
#endif

	return ret;
}
