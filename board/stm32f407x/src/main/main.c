/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <board.h>
#include <core.h>
// #include <hmc5883l_task.h>
#include <led_task.h>
#include <std.h>
#include <sysclk.h>

int main(int argc, char* argv[])
{
	board_init();
	
	// serial1_init();
	kernel_startup();
	std_init();

	led_task();
	// // hmc5883l_task();

	sysclk_init();

	while (1)
	{
	}
}
