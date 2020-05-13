/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "streams.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vsnprintf
 ****************************************************************************/

int vsnprintf(FAR char *buf, size_t size, FAR const char *format,
			  va_list ap)
{
	union {
		struct lib_outstream_s nulloutstream;
		struct lib_memoutstream_s memoutstream;
	} u;

	FAR struct lib_outstream_s *stream;
	int n;

	/* "If the value of [size] is zero on a call to vsnprintf(), nothing shall
   *  be written, the number of bytes that would have been written had [size]
   *  been sufficiently large excluding the terminating null shall be returned,
   *  and [buf] may be a null pointer." -- opengroup.org
   */

	if (size > 0)
	{
		/* Initialize a memory stream to write to the buffer */

		lib_memoutstream(&u.memoutstream, buf, size);
		stream = &u.memoutstream.public;
	}
	else
	{
		/* Use a null stream to get the size of the buffer */

		lib_nulloutstream(&u.nulloutstream);
		stream = &u.nulloutstream;
	}

	/* Then let lib_vsprintf do the real work */

	n = lib_vsprintf(stream, format, ap);
	return n;
}
