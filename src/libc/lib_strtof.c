#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <libc.h>

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

/* These are predefined with GCC, but could be issues for other compilers. If
 * not defined, an arbitrary big number is put in for now.  These should be
 * added to nuttx/compiler for your compiler.
 */

#if !defined(__FLT_MIN_EXP__) || !defined(__FLT_MAX_EXP__)
#  ifdef CONFIG_CPP_HAVE_WARNING
#    warning "Size of exponent is unknown"
#  endif
#  undef  __FLT_MIN_EXP__
#  define __FLT_MIN_EXP__ (-125)
#  undef  __FLT_MAX_EXP__
#  define __FLT_MAX_EXP__ (128)
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline int is_real(float x)
{
  const float infinite = 1.0F/0.0F;
  return (x < infinite) && (x >= -infinite);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/***************************************************(************************
 * Name: strtof
 *
 * Description:
 *   Convert a string to a float value
 *
 ****************************************************************************/

float strtof(FAR const char *str, FAR char **endptr)
{
  float number;
  int exponent;
  int negative;
  FAR char *p = (FAR char *) str;
  float p10;
  int n;
  int num_digits;
  int num_decimals;
  const float infinite = 1.0F/0.0F;

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

  number       = 0.0F;
  exponent     = 0;
  num_digits   = 0;
  num_decimals = 0;

  /* Process string of digits */

  while (isdigit(*p))
    {
      number = number * 10.0F + (float)(*p - '0');
      p++;
      num_digits++;
    }

  /* Process decimal part */

  if (*p == '.')
    {
      p++;

      while (isdigit(*p))
        {
          number = number * 10.0F + (float)(*p - '0');
          p++;
          num_digits++;
          num_decimals++;
        }

      exponent -= num_decimals;
    }

  if (num_digits == 0)
    {
      set_errno(ERANGE);
      number = 0.0F;
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
          negative = 1;   /* Fall through to increment pos */
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

  if (exponent < __FLT_MIN_EXP__ ||
      exponent > __FLT_MAX_EXP__)
    {
      set_errno(ERANGE);
      number = infinite;
      goto errout;
    }

  /* Scale the result */

  p10 = 10.0F;
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
