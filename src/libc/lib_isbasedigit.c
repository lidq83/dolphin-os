#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <libc.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lib_isbasedigit
 *
 * Description:
 *   Given an ASCII character, ch, and a base (1-36) do two
 *   things:  1) Determine if ch is a valid charcter, and 2)
 *   convert ch to its binary value.
 *
 ****************************************************************************/

bool lib_isbasedigit(int ch, int base, int *value)
{
	bool ret = false;
	int tmp = 0;

	if (base <= 10)
	{
		if (ch >= '0' && ch <= base + '0' - 1)
		{
			tmp = ch - '0';
			ret = true;
		}
	}
	else if (base <= 36)
	{
		if (ch >= '0' && ch <= '9')
		{
			tmp = ch - '0';
			ret = true;
		}
		else if (ch >= 'a' && ch <= 'a' + base - 11)
		{
			tmp = ch - 'a' + 10;
			ret = true;
		}
		else if (ch >= 'A' && ch <= 'A' + base - 11)
		{
			tmp = ch - 'A' + 10;
			ret = true;
		}
	}

	if (value)
	{
		*value = tmp;
	}
	return ret;
}
