/****************************************************************************
 * Included Files
 ****************************************************************************/


#include "streams.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void nulloutstream_putc(FAR struct lib_outstream_s *this, int ch)
{
  this->nput++;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lib_nulloutstream
 *
 * Description:
 *   Initializes a NULL streams. The initialized stream will write all data
 *   to the bit-bucket.
 *
 * Input parameters:
 *   nulloutstream - User allocated, uninitialized instance of struct
 *                   lib_outstream_s to be initialized.
 *
 * Returned Value:
 *   None (User allocated instance initialized).
 *
 ****************************************************************************/

void lib_nulloutstream(FAR struct lib_outstream_s *nulloutstream)
{
  nulloutstream->put   = nulloutstream_putc;
  nulloutstream->flush = lib_noflush;
  nulloutstream->nput  = 0;
}

