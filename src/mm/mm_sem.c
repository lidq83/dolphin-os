/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include <mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Define the following to enable semaphore state monitoring */
//#define MONITOR_MM_SEMAPHORE 1

#ifdef MONITOR_MM_SEMAPHORE
#include <debug.h>
#define msemerr _err
#define msemwarn _warn
#define mseminfo _info
#else
#ifdef CONFIG_CPP_HAVE_VARARGS
#define msemerr(x...)
#define msemwarn(x...)
#define mseminfo(x...)
#else
#define msemerr (void)
#define msemwarn (void)
#define mseminfo (void)
#endif
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_seminitialize
 *
 * Description:
 *   Initialize the MM mutex
 *
 ****************************************************************************/

void mm_seminitialize(FAR struct mm_heap_s *heap)
{
	/* Initialize the MM semaphore to one (to support one-at-a-time access to
   * private data sets).
   */
	sem_init(&heap->mm_semaphore, 1);
}

/****************************************************************************
 * Name: mm_trysemaphore
 *
 * Description:
 *   Try to take the MM mutex.  This is called only from the OS in certain
 *   conditions when it is necessary to have exclusive access to the memory
 *   manager but it is impossible to wait on a semaphore (e.g., the idle
 *   process when it performs its background memory cleanup).
 *
 ****************************************************************************/

int mm_trysemaphore(FAR struct mm_heap_s *heap)
{
	sem_wait(&heap->mm_semaphore);
	return OK;
}

/****************************************************************************
 * Name: mm_takesemaphore
 *
 * Description:
 *   Take the MM mutex.  This is the normal action before all memory
 *   management actions.
 *
 ****************************************************************************/

void mm_takesemaphore(FAR struct mm_heap_s *heap)
{
	sem_wait(&heap->mm_semaphore);
}

/****************************************************************************
 * Name: mm_givesemaphore
 *
 * Description:
 *   Release the MM mutex when it is not longer needed.
 *
 ****************************************************************************/

void mm_givesemaphore(FAR struct mm_heap_s *heap)
{
	sem_post(&heap->mm_semaphore);
}
