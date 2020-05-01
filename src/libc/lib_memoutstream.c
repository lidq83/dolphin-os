/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "streams.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: memoutstream_putc
 ****************************************************************************/

static void memoutstream_putc(FAR struct lib_outstream_s *this, int ch)
{
	FAR struct lib_memoutstream_s *mthis = (FAR struct lib_memoutstream_s *)this;

	/* If this will not overrun the buffer, then write the character to the
   * buffer.  Not that buflen was pre-decremented when the stream was
   * created so it is okay to write past the end of the buflen by one.
   */

	if (this->nput < mthis->buflen)
	{
		mthis->buffer[this->nput] = ch;
		this->nput++;
		mthis->buffer[this->nput] = '\0';
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lib_memoutstream
 *
 * Description:
 *   Initializes a stream for use with a fixed-size memory buffer.
 *
 * Input parameters:
 *   outstream - User allocated, uninitialized instance of struct
 *                  lib_memoutstream_s to be initialized.
 *   bufstart     - Address of the beginning of the fixed-size memory buffer
 *   buflen       - Size of the fixed-sized memory buffer in bytes
 *
 * Returned Value:
 *   None (outstream initialized).
 *
 ****************************************************************************/

void lib_memoutstream(FAR struct lib_memoutstream_s *outstream,
					  FAR char *bufstart, int buflen)
{
	outstream->public.put = memoutstream_putc;
	outstream->public.flush = lib_noflush;
	outstream->public.nput = 0;		/* Will be buffer index */
	outstream->buffer = bufstart;	/* Start of buffer */
	outstream->buflen = buflen - 1; /* Save space for null terminator */
	outstream->buffer[0] = '\0';	/* Start with an empty string */
}
