#include <stdlib.h>
#include <errno.h>
#include <libc.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: strtoul
 *
 * Description:
 *   The strtoul() function  converts  the initial part of the string in
 *   nptr to a long unsigned integer value according to the given base, which
 *   must be between 2 and 36 inclusive, or be the special value 0.
 *
 * Returns:
 *   - The converted value, if the base and number are valid
 *   - 0 if an error occurs, and set errno to:
 *     * EINVAL if base < 2 or base > 36
 *   - ULONG_MAX if an overflow occurs, and set errno to:
 *     * ERANGE if the number cannot be represented using unsigned long
 *
 ****************************************************************************/

unsigned long strtoul(FAR const char *nptr, FAR char **endptr, int base)
{
	unsigned long accum = 0;
	unsigned long prev;
	int value;

	if (nptr)
	{
		/* Skip leading spaces */

		lib_skipspace(&nptr);

		/* Check for unspecified or incorrect base */

		base = lib_checkbase(base, &nptr);

		if (base < 0)
		{
			set_errno(EINVAL);
			return 0;
		}

		/* Accumulate each "digit" */

		while (lib_isbasedigit(*nptr, base, &value))
		{
			prev = accum;
			accum = accum * base + value;
			nptr++;

			/* Check for overflow */

			if (accum < prev)
			{
				set_errno(ERANGE);
				accum = ULONG_MAX;
				break;
			}
		}

		/* Return the final pointer to the unused value */

		if (endptr)
		{
			*endptr = (char *)nptr;
		}
	}

	return accum;
}
