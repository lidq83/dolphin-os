/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <libc.h>

#ifdef CONFIG_HAVE_DOUBLE

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

/* These are predefined with GCC, but could be issues for other compilers. If
 * not defined, an arbitrary big number is put in for now.  These should be
 * added to nuttx/compiler for your compiler.
 */

#if !defined(__DBL_MIN_EXP__) || !defined(__DBL_MAX_EXP__)
#ifdef CONFIG_CPP_HAVE_WARNING
#warning "Size of exponent is unknown"
#endif
#undef __DBL_MIN_EXP__
#define __DBL_MIN_EXP__ (-1021)
#undef __DBL_MAX_EXP__
#define __DBL_MAX_EXP__ (1024)
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline int is_real(double x)
{
	const double infinite = 1.0 / 0.0;
	return (x < infinite) && (x >= -infinite);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/***************************************************(************************
 * Name: strtod
 *
 * Description:
 *   Convert a string to a double value
 *
 ****************************************************************************/

double strtod(FAR const char *str, FAR char **endptr)
{
	double number;
	int exponent;
	int negative;
	FAR char *p = (FAR char *)str;
	double p10;
	int n;
	int num_digits;
	int num_decimals;
	const double infinite = 1.0 / 0.0;

	/* Skip leading whitespace */

	while (isspace(*p))
	{
		p++;
	}

	/* Handle optional sign */

	negative = 0;
	switch (*p)
	{
	case '-':
		negative = 1; /* Fall through to increment position */
					  /* FALLTHROUGH */
	case '+':
		p++;
		/* FALLTHROUGH */
	default:
		break;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	/* Process string of digits */

	while (isdigit(*p))
	{
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	/* Process decimal part */

	if (*p == '.')
	{
		p++;

		while (isdigit(*p))
		{
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}

		exponent -= num_decimals;
	}

	if (num_digits == 0)
	{
		set_errno(ERANGE);
		number = 0.0;
		goto errout;
	}

	/* Correct for sign */

	if (negative)
	{
		number = -number;
	}

	/* Process an exponent string */

	if (*p == 'e' || *p == 'E')
	{
		/* Handle optional sign */

		negative = 0;
		switch (*++p)
		{
		case '-':
			negative = 1; /* Fall through to increment pos */
						  /* FALLTHROUGH */
		case '+':
			p++;
			/* FALLTHROUGH */
		default:
			break;
		}

		/* Process string of digits */

		n = 0;
		while (isdigit(*p))
		{
			n = n * 10 + (*p - '0');
			p++;
		}

		if (negative)
		{
			exponent -= n;
		}
		else
		{
			exponent += n;
		}
	}

	if (exponent < __DBL_MIN_EXP__ ||
		exponent > __DBL_MAX_EXP__)
	{
		set_errno(ERANGE);
		number = infinite;
		goto errout;
	}

	/* Scale the result */

	p10 = 10.;
	n = exponent;
	if (n < 0)
	{
		n = -n;
	}

	while (n)
	{
		if (n & 1)
		{
			if (exponent < 0)
			{
				number /= p10;
			}
			else
			{
				number *= p10;
			}
		}

		n >>= 1;
		p10 *= p10;
	}

	if (!is_real(number))
	{
		set_errno(ERANGE);
	}

errout:
	if (endptr)
	{
		*endptr = p;
	}

	return number;
}

#endif /* CONFIG_HAVE_DOUBLE */
