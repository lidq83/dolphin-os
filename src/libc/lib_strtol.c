#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <libc.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: strtol
 *
 * Description:
 *   The  strtol() function  converts  the initial part of the string in
 *   nptr to a long integer value according to the given base, which must be
 *   between 2 and 36 inclusive, or be the special value 0.
 *
 * Returns:
 *   - The converted value, if the base and number are valid
 *   - 0 if an error occurs, and set errno to:
 *     * EINVAL if base < 2 or base > 36
 *   - LONG_MIN or LONG_MAX, of correct sign, if an overflow occurs,
 *     and set errno to:
 *     * ERANGE if the number cannot be represented using long
 *
 ****************************************************************************/

long strtol(FAR const char *nptr, FAR char **endptr, int base)
{
	unsigned long accum = 0;
	bool negate = false;

	if (nptr)
	{
		/* Skip leading spaces */

		lib_skipspace(&nptr);

		/* Check for leading + or - */

		if (*nptr == '-')
		{
			negate = true;
			nptr++;
		}
		else if (*nptr == '+')
		{
			nptr++;
		}

		/* Get the unsigned value */

		accum = strtoul(nptr, endptr, base);

		/* Correct the sign of the result and check for overflow */

		if (negate)
		{
			const unsigned long limit = ((unsigned long)-(LONG_MIN + 1)) + 1;

			if (accum > limit)
			{
				set_errno(ERANGE);
				return LONG_MIN;
			}

			return (accum == limit) ? LONG_MIN : -(long)accum;
		}

		if (accum > LONG_MAX)
		{
			set_errno(ERANGE);
			return LONG_MAX;
		}
	}

	return (long)accum;
}
