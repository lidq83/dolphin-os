#include <string.h>
#include <ctype.h>

#include <libc.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lib_checkbase
 *
 * Description:
 *   This is part of the strol() family implementation.  This function checks
 *   the initial part of a string to see if it can determine the numeric
 *   base that is represented.
 *
 * Assumptions:
 *   *ptr points to the first, non-whitespace character in the string.
 *
 * Returns:
 *   - if base is valid, the actual base to use, and pptr is updated to point
 *     at the first digit.
 *   - if base is invalid (<2 or >36), return -1.
 *
 ****************************************************************************/

int lib_checkbase(int base, FAR const char **pptr)
{
	FAR const char *ptr = *pptr;

	/* Check for unspecified base */

	if (!base)
	{
		/* Assume base 10 */

		base = 10;

		/* Check for leading '0' - that would signify octal or hex (or binary) */

		if (*ptr == '0')
		{
			/* Assume octal */

			base = 8;
			ptr++;

			/* Check for hexadecimal */

			if ((*ptr == 'X' || *ptr == 'x') &&
				lib_isbasedigit(ptr[1], 16, NULL))
			{
				base = 16;
				ptr++;
			}
		}
	}

	/* If it a hexadecimal representation, than discard any leading "0X" or "0x" */

	else if (base == 16)
	{
		if (ptr[0] == '0' && (ptr[1] == 'X' || ptr[1] == 'x'))
		{
			ptr += 2;
		}
	}

	/* Check for incorrect bases. */

	else if (base < 2 || base > 26)
	{
		return -1; /* Means incorrect base */
	}

	/* Return the updated pointer and base */

	*pptr = ptr;
	return base;
}
