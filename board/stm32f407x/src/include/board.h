#ifndef __BOARD_H_
#define __BOARD_H_

#ifndef MM_SIZE_HEAP
#define MM_SIZE_HEAP (1024 * 112)
#endif

#define MM_REGION2
#define MM_REGION2_ADDR (0x10000000) // ~0x1000FFFF
#define MM_REGION2_SIZE (0x10000) //64KB

#define SERIAL1_BAUTRATE (115200)

#endif