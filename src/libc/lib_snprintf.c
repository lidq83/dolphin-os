/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "streams.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * sprintf
 ****************************************************************************/

int snprintf(FAR char *buf, size_t size, FAR const char *format, ...)
{
	union {
		struct lib_outstream_s nulloutstream;
		struct lib_memoutstream_s memoutstream;
	} u;

	FAR struct lib_outstream_s *stream;
	va_list ap;
	int n;

	/* "If the value of [size] is zero on a call to snprintf(), nothing shall
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

	va_start(ap, format);
	n = lib_vsprintf(stream, format, ap);
	va_end(ap);
	return n;
}
