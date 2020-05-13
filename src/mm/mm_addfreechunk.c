/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <mm.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_addfreechunk
 *
 * Description:
 *   Add a free chunk to the node next.  It is assumed that the caller holds
 *   the mm semaphore
 *
 ****************************************************************************/

void mm_addfreechunk(FAR struct mm_heap_s *heap, FAR struct mm_freenode_s *node)
{
	FAR struct mm_freenode_s *next;
	FAR struct mm_freenode_s *prev;

	/* Convert the size to a nodelist index */

	int ndx = mm_size2ndx(node->size);

	/* Now put the new node int the next */

	for (prev = &heap->mm_nodelist[ndx], next = heap->mm_nodelist[ndx].flink;
		 next && next->size && next->size < node->size;
		 prev = next, next = next->flink)
		;

	/* Does it go in mid next or at the end? */

	prev->flink = node;
	node->blink = prev;
	node->flink = next;

	if (next)
	{
		/* The new node goes between prev and next */

		next->blink = node;
	}
}
