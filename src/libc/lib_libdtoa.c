
#include "streams.h"

extern char *__dtoa(double d, int mode, int ndigits, int *decpt, int *sign, char **rve);

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MAX_PREC 16

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: zeroes
 *
 * Description:
 *   Print the specified number of zeres
 *
 ****************************************************************************/

static void zeroes(FAR struct lib_outstream_s *obj, int nzeroes)
{
	int i;

	for (i = nzeroes; i > 0; i--)
	{
		obj->put(obj, '0');
	}
}

/****************************************************************************
 * Name: lib_dtoa_string
 *
 * Description:
 *   Print the specified string
 *
 ****************************************************************************/

static void lib_dtoa_string(FAR struct lib_outstream_s *obj, const char *str)
{
	while (*str)
	{
		obj->put(obj, *str++);
	}
}

/****************************************************************************
 * Name: lib_dtoa
 *
 * Description:
 *   This is part of lib_vsprintf().  It handles the floating point formats.
 *   This version supports only the %f (with precision).  If no precision
 *   was provided in the format, this will use precision == 0 which is
 *   probably not what you want.
 *
 * Input Parameters:
 *   obj   - The output stream object
 *   fmt   - The format character.  Not used 'f' is always assumed
 *   prec  - The number of digits to the right of the decimal point. If no
 *           precision is provided in the format, this will be zero.  And,
 *           unfortunately in this case, it will be treated literally as
 *           a precision of zero.
 *   flags - Only ALTFORM and SHOWPLUS flags are supported.  ALTFORM only
 *           applies if prec == 0 which is not supported anyway.
 *   value - The floating point value to convert.
 *
 ****************************************************************************/

static void lib_dtoa(FAR struct lib_outstream_s *obj, int fmt, int prec,
					 uint8_t flags, double value)
{
	FAR char *digits; /* String returned by __dtoa */
	FAR char *rve;	  /* Points to the end of the return value */
	int expt;		  /* Integer value of exponent */
	int numlen;		  /* Actual number of digits returned by cvt */
	int nchars;		  /* Number of characters to print */
	int dsgn;		  /* Unused sign indicator */
	int i;

	/* This function may *NOT* be called within interrupt level logic.  That is
   * because the logic in __dtoa may attempt to allocate memory.  That will
   * lead to cryptic failures down the road within the memory manager.
   * Better to explicitly assert upstream here.  Rule:  Don't use floating
   * point formats on any output from interrupt handling logic.
   */

	/* Special handling for NaN and Infinity */

	if (isnan(value))
	{
		lib_dtoa_string(obj, "NaN");
		return;
	}

	if (isinf(value))
	{
		if (value < 0.0)
		{
			obj->put(obj, '-');
		}

		lib_dtoa_string(obj, "Infinity");
		return;
	}

	/* Non-zero... positive or negative */

	if (value < 0)
	{
		value = -value;
		SET_NEGATE(flags);
	}

	/* Perform the conversion */

	digits = __dtoa(value, 3, prec, &expt, &dsgn, &rve);
	numlen = rve - digits;

	/* Avoid precision error from missing trailing zeroes */

	numlen = MAX(expt, numlen);

	if (IS_NEGATE(flags))
	{
		obj->put(obj, '-');
	}
	else if (IS_SHOWPLUS(flags))
	{
		obj->put(obj, '+');
	}

	/* Special case exact zero or the case where the number is smaller than
   * the print precision.
   */

	if (value == 0 || expt < -prec)
	{
		/* kludge for __dtoa irregularity */

		obj->put(obj, '0');

		/* A decimal point is printed only in the alternate form or if a
       * particular precision is requested.
       */

		if (prec > 0 || IS_ALTFORM(flags))
		{
			obj->put(obj, '.');

			/* Always print at least one digit to the right of the decimal point. */

			prec = MAX(1, prec);
		}
	}
	/* A non-zero value will be printed */
	else
	{
		/* Handle the case where the value is less than 1.0 (in magnitude) and
       * will need a leading zero.
       */
		if (expt <= 0)
		{
			/* Print a single zero to the left of the decimal point */

			obj->put(obj, '0');

			/* Print the decimal point */

			obj->put(obj, '.');

			/* Print any leading zeros to the right of the decimal point */

			if (expt < 0)
			{
				nchars = MIN(-expt, prec);
				zeroes(obj, nchars);
				prec -= nchars;
			}
		}

		/* Handle the general case where the value is greater than 1.0 (in
       * magnitude).
       */

		else
		{
			/* Print the integer part to the left of the decimal point */

			for (i = expt; i > 0; i--)
			{
				if (*digits != '\0')
				{
					obj->put(obj, *digits);
					digits++;
				}
				else
				{
					obj->put(obj, '0');
				}
			}

			/* Get the length of the fractional part */

			numlen -= expt;

			/* If there is no fractional part, then a decimal point is printed
           * only in the alternate form or if a particular precision is
           * requested.
           */

			if (numlen > 0 || prec > 0 || IS_ALTFORM(flags))
			{
				/* Print the decimal point */

				obj->put(obj, '.');

				/* Always print at least one digit to the right of the decimal
               * point.
               */

				prec = MAX(1, prec);
			}
		}

		/* If a precision was specified, then limit the number digits to the
       * right of the decimal point.
       */

		if (prec > 0)
		{
			nchars = MIN(numlen, prec);
		}
		else
		{
			nchars = numlen;
		}

		/* Print the fractional part to the right of the decimal point */

		for (i = nchars; i > 0; i--)
		{
			obj->put(obj, *digits);
			digits++;
		}

		/* Decrement to get the number of trailing zeroes to print */

		prec -= nchars;
	}

	/* Finally, print any trailing zeroes */

	zeroes(obj, prec);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
