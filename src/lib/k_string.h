/*
 * k_string.h
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#ifndef __SRC_BOARD_K_STRING_H
#define __SRC_BOARD_K_STRING_H

void k_memset(void *mem, int val, unsigned long size);

void k_memcpy(void *tar, void *src, unsigned long size);

int k_strcpy(char *tar, char *src);

int k_strncpy(char *tar, char *src, int len);

int k_strcmp(char *str1, char *str2);

int k_strlen(char *str);

#endif
