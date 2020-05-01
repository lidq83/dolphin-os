#ifndef __BUFF_S_H
#define __BUFF_S_H

#define BUFF_SIZE_UART (128)

typedef struct buff_s
{
	char buff[BUFF_SIZE_UART];
	int head;
	int foot;
} buff_s;

int buff_append(buff_s *buff, char ch);

int buff_size(buff_s *buff);

#endif