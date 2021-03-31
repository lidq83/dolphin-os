/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <hmc5883l.h>
#include <hmc5883l_task.h>
#include <math.h>

void* hmc5883l_pthread(void* arg)
{
	// struct mallinfo info = { 0 };
	// mallinfo(&info);
	// k_printf("%d %d %d %d %d\n", info.arena, info.ordblks, info.mxordblk, info.uordblks, info.fordblks);

	k_printf("I2c init\n");
	i2c2_init();

	k_printf("hmc5883l init\n");
	HMC5883L_Initialize();

	int ret = HMC5883L_TestConnection();
	if (ret == 0)
	{
		k_printf("hmc5883l init Error\n");
		return NULL;
	}
	k_printf("hmc5883l init OK\n");

	uint32_t tk = 0;

	short mag[3] = { 0 };
	int cycle_a = 0;
	float filter = 0.1;
	float last_a = 0;
	float angle = 0;
	float angle_last = 0;

	while (1)
	{
		HMC5883L_GetHeading(mag);
		// k_printf("%+6d %+6d\n", mag[0], mag[2]);

		float a = atan2((double)mag[2], (double)mag[0]);
		if (a < 0)
		{
			a += 2 * M_PI;
		}

		if (a - last_a < -M_PI)
		{
			cycle_a++;
		}
		else if (a - last_a > M_PI)
		{
			cycle_a--;
		}
		last_a = a;
		float ang = a + (cycle_a * 2 * M_PI);
		angle = ang * filter + angle_last * (1.0 - filter);
		angle_last = angle;

		k_printf("%+8.3f\n", angle * 180.0 / M_PI);
		tk++;

		sleep_ticks(25);
	}

	return NULL;
}

void hmc5883l_task(void)
{
	pcb_create(PRIO_HMC5883L, &hmc5883l_pthread, NULL, 800);
}
