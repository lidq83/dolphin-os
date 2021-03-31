#ifndef __BUFF_S_H
#define __BUFF_S_H

#include <board.h>

#define BUFF_SIZE (256)

typedef struct buff_s
{
	char buff[BUFF_SIZE];
	int head;
	int foot;
} buff_s;

int buff_append(buff_s* buff, char ch);

int buff_size(buff_s* buff);

#endif