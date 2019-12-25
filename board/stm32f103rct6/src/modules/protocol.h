/*
 * protocol.h
 *
 *  Created on: Sep 27, 2018
 *      Author: lidq
 */

#ifndef __SRC_MODULES_PROTOCOL_H_
#define __SRC_MODULES_PROTOCOL_H_

#include <typedef.h>

#define P_TYPE_DISTANCE			0x0030
#define P_TYPE_MOTION			0x0040

#define P_BUFF_SIZE				(256)

#define P_HEAD_0				0xEE
#define P_HEAD_1				0x66

#define P_FOOT_0				0xE6
#define P_FOOT_1				0x6E

#define P_SIZE_HEAD				(2)
#define P_SIZE_SEQ				(2)
#define P_SIZE_TYPE				(2)
#define P_SIZE_LEN				(4)
#define P_SIZE_CRC				(4)
#define P_SIZE_FOOT				(2)

#define P_PRO_LEN_NO_DATA		(P_SIZE_HEAD + P_SIZE_SEQ + P_SIZE_TYPE + P_SIZE_LEN + P_SIZE_CRC + P_SIZE_FOOT)

#define CRC32_SEED_VAL			(0xEDB88320L)

typedef struct buff_s
{
	uint8_t buff[P_BUFF_SIZE];
	int head;
	int foot;
} buff_s;

int protocol_pack(uint8_t *p_out, uint32_t *out_len, uint8_t *p_data, uint16_t data_seq, uint16_t data_type, uint32_t data_len);

int protocol_unpack(uint8_t *p_out, uint32_t *out_len, uint16_t *data_seq, uint16_t *data_type, buff_s *p_buff);

uint32_t protocol_crc32value(uint32_t i);

uint32_t protocol_calc32crc(uint8_t *p_buff, uint32_t len);

int protocol_buff_append(buff_s *p_buff, uint8_t *p_data, uint32_t len);

int protocol_buff_cnt(buff_s *p_buff);

#endif /* SRC_MODULES_DATA_SCHED_DS_CORE_PROTOCOL_H_ */
