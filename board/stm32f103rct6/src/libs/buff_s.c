#include <buff_s.h>

int buff_append(buff_s *buff, char ch)
{
	buff->buff[buff->head] = ch;
	buff->head++;
	buff->head %= BUFF_SIZE_UART;
	if (buff->head == buff->foot)
	{
		buff->foot++;
		buff->foot %= BUFF_SIZE_UART;
	}
	return 0;
}

int buff_size(buff_s *buff)
{
	int size = buff->head - buff->foot;
	if (size < 0)
	{
		size += BUFF_SIZE_UART;
	}
	return size;
}