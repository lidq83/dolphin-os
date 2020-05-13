/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <mm.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_free
 *
 * Description:
 *   Returns a chunk of memory to the list of free nodes,  merging with
 *   adjacent free chunks if possible.
 *
 ****************************************************************************/

void mm_free(FAR struct mm_heap_s *heap, FAR void *mem)
{
	FAR struct mm_freenode_s *node;
	FAR struct mm_freenode_s *prev;
	FAR struct mm_freenode_s *next;

	/* Protect against attempts to free a NULL reference */

	if (!mem)
	{
		return;
	}

	/* We need to hold the MM semaphore while we muck with the
   * nodelist.
   */

	mm_takesemaphore(heap);

	/* Map the memory chunk into a free node */

	node = (FAR struct mm_freenode_s *)((FAR char *)mem - SIZEOF_MM_ALLOCNODE);
	node->preceding &= ~MM_ALLOC_BIT;

	/* Check if the following node is free and, if so, merge it */

	next = (FAR struct mm_freenode_s *)((FAR char *)node + node->size);
	if ((next->preceding & MM_ALLOC_BIT) == 0)
	{
		FAR struct mm_allocnode_s *andbeyond;

		/* Get the node following the next node (which will
       * become the new next node). We know that we can never
       * index past the tail chunk because it is always allocated.
       */

		andbeyond = (FAR struct mm_allocnode_s *)((FAR char *)next + next->size);

		/* Remove the next node.  There must be a predecessor,
       * but there may not be a successor node.
       */

		next->blink->flink = next->flink;
		if (next->flink)
		{
			next->flink->blink = next->blink;
		}

		/* Then merge the two chunks */

		node->size += next->size;
		andbeyond->preceding = node->size | (andbeyond->preceding & MM_ALLOC_BIT);
		next = (FAR struct mm_freenode_s *)andbeyond;
	}

	/* Check if the preceding node is also free and, if so, merge
   * it with this node
   */

	prev = (FAR struct mm_freenode_s *)((FAR char *)node - node->preceding);
	if ((prev->preceding & MM_ALLOC_BIT) == 0)
	{
		/* Remove the node.  There must be a predecessor, but there may
       * not be a successor node.
       */

		prev->blink->flink = prev->flink;
		if (prev->flink)
		{
			prev->flink->blink = prev->blink;
		}

		/* Then merge the two chunks */

		prev->size += node->size;
		next->preceding = prev->size | (next->preceding & MM_ALLOC_BIT);
		node = prev;
	}

	/* Add the merged node to the nodelist */

	mm_addfreechunk(heap, node);
	mm_givesemaphore(heap);
}
