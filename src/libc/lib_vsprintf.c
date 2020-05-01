/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "streams.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vsprintf
 ****************************************************************************/

int vsprintf(FAR char *dest, FAR const char *src, va_list ap)
{
  struct lib_memoutstream_s memoutstream;

  /* Wrap the destination buffer in a stream object and let
   * libc/stdio/lib_vsprintf do the work.
   */

  lib_memoutstream((FAR struct lib_memoutstream_s *)&memoutstream, dest,
                   LIB_BUFLEN_UNKNOWN);
  return lib_vsprintf((FAR struct lib_outstream_s *)&memoutstream.public,
                      src, ap);
}
