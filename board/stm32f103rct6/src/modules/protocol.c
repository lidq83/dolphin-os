/*
 * protocol.c
 *
 *  Created on: Sep 27, 2018
 *      Author: lidq
 */

#include <protocol.h>

int protocol_pack(uint8_t *p_out, uint32_t *out_len, uint8_t *p_data, uint16_t data_seq, uint16_t data_type, uint32_t data_len)
{
	if (p_out == NULL)
	{
		return -1;
	}

	if (p_data == NULL)
	{
		return -1;
	}

	if (data_len == 0)
	{
		return -1;
	}

	if (data_len > P_BUFF_SIZE)
	{
		return -1;
	}

	int pos = 0;

	//head
	p_out[pos] = P_HEAD_0;
	pos++;
	p_out[pos] = P_HEAD_1;
	pos++;

	//seq
	p_out[pos] = (data_seq >> 0) & 0xFF;
	pos++;
	p_out[pos] = (data_seq >> 8) & 0xFF;
	pos++;

	//type
	p_out[pos] = (data_type >> 0) & 0xFF;
	pos++;
	p_out[pos] = (data_type >> 8) & 0xFF;
	pos++;

	//length
	p_out[pos] = (data_len >> 0) & 0xFF;
	pos++;
	p_out[pos] = (data_len >> 8) & 0xFF;
	pos++;
	p_out[pos] = (data_len >> 16) & 0xFF;
	pos++;
	p_out[pos] = (data_len >> 24) & 0xFF;
	pos++;

	//data
	for (int i = 0; i < data_len; i++)
	{
		p_out[pos] = p_data[i];
		pos++;
	}

	//crc
	uint32_t crc = protocol_calc32crc(p_out, pos);
	p_out[pos] = (crc >> 0) & 0xFF;
	pos++;
	p_out[pos] = (crc >> 8) & 0xFF;
	pos++;
	p_out[pos] = (crc >> 16) & 0xFF;
	pos++;
	p_out[pos] = (crc >> 24) & 0xFF;
	pos++;

	//foot
	p_out[pos] = P_FOOT_0;
	pos++;
	p_out[pos] = P_FOOT_1;
	pos++;

	*out_len = pos;

	return 0;
}

int protocol_unpack(uint8_t *p_out, uint32_t *out_len, uint16_t *data_seq, uint16_t *data_type, buff_s *p_buff)
{
	if (p_out == NULL)
	{
		return -1;
	}

	if (out_len == NULL)
	{
		return -1;
	}

	if (p_buff == NULL)
	{
		return -1;
	}

	uint16_t ret_seq = 0;
	uint16_t ret_type = 0;
	int ret_len = 0;
	int ret_ind = 0;
	uint32_t ret_crc = 0;
	int ret_finish = 0;

	int step = 0;
	int cnt = protocol_buff_cnt(p_buff);
	if (cnt <= 0)
	{
		return -1;
	}

	int foot = p_buff->foot;
	for (int i = 0; i < cnt; i++)
	{
		uint8_t ch = p_buff->buff[foot];
		foot++;
		foot %= P_BUFF_SIZE;

		switch (step)
		{
			case 0: //HEAD_0
			{
				ret_ind = 0;
				step = 0;
				if (ch == P_HEAD_0)
				{
					p_out[ret_ind] = ch;
					ret_ind++;
					step = 1;
				}
				break;
			}
			case 1: //HEAD_1
			{
				step = 0;
				if (ch == P_HEAD_1)
				{
					p_out[ret_ind] = ch;
					ret_ind++;
					step = 2;
				}
				break;
			}
			case 2: //SEQ_0
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_seq = (ch << 0);
				step = 3;
				break;
			}
			case 3: //SEQ_1
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_seq |= (ch << 8);
				step = 4;
				break;
			}
			case 4: //TYPE_0
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_type = (ch << 0);
				step = 5;
				break;
			}
			case 5: //TYPE_1
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_type |= (ch << 8);
				step = 6;
				break;
			}
			case 6: //DATA_LEN_HIGH_8BITS
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_len = (ch << 0);
				step = 7;
				break;
			}
			case 7: //DATA_LEN_LOW_8BITS
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_len |= (ch << 8);
				step = 8;
				break;
			}
			case 8: //DATA_LEN_HIGH_8BITS
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_len |= (ch << 16);
				step = 9;
				break;
			}
			case 9: //DATA_LEN_LOW_8BITS
			{
				p_out[ret_ind] = ch;
				ret_ind++;
				ret_len |= (ch << 24);
				step = 10;
				break;
			}
			case 10: //DATA
			{
				step = 10;
				p_out[ret_ind] = ch;
				ret_ind++;
				if (ret_ind >= ret_len + P_SIZE_HEAD + P_SIZE_SEQ + P_SIZE_TYPE + P_SIZE_LEN)
				{
					step = 11;
				}
				break;
			}
			case 11: //CRC_0
			{
				ret_crc = (ch << 0);
				step = 12;
				break;
			}
			case 12: //CRC_1
			{
				ret_crc |= (ch << 8);
				step = 13;
				break;
			}
			case 13: //CRC_2
			{
				ret_crc |= (ch << 16);
				step = 14;
				break;
			}
			case 14: //CRC_3
			{
				ret_crc |= (ch << 24);
				step = 15;
				break;
			}
			case 15: //FOOT_0
			{
				step = 0;
				if (ch == P_FOOT_0)
				{
					step = 16;
				}
				break;
			}
			case 16: //FOOT_1
			{
				step = 0;
				if (ch == P_FOOT_1)
				{
					ret_finish = 1;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		if (ret_finish)
		{
			break;
		}
	}

	if (!ret_finish)
	{
		return -1;
	}

	*out_len = ret_len;
	*data_type = ret_type;
	*data_seq = ret_seq;

	uint32_t crc = protocol_calc32crc(p_out, ret_ind);
	p_buff->foot = foot;

	if (crc != ret_crc)
	{
		return -1;
	}

	uint8_t *p = p_out;
	uint8_t *q = p_out + P_SIZE_HEAD + P_SIZE_SEQ + P_SIZE_TYPE + P_SIZE_LEN;
	for (int i = 0; i < ret_len; i++)
	{
		*p++ = *q++;
	}

	return 0;
}

int protocol_buff_append(buff_s *p_buff, uint8_t *p_data, uint32_t len)
{
	if (p_buff == NULL)
	{
		return -1;
	}

	if (p_data == NULL)
	{
		return -1;
	}

	if (len <= 0)
	{
		return -1;
	}

	for (int i = 0; i < len; i++)
	{
		p_buff->buff[p_buff->head] = p_data[i];
		p_buff->head++;
		p_buff->head %= P_BUFF_SIZE;
		if (p_buff->head == p_buff->foot)
		{
			p_buff->foot++;
			p_buff->foot %= P_BUFF_SIZE;
		}
	}
	return 0;
}

int protocol_buff_cnt(buff_s *p_buff)
{
	if (p_buff == NULL)
	{
		return -1;
	}

	int len = p_buff->head - p_buff->foot;
	if (len < 0)
	{
		len += P_BUFF_SIZE;
	}
	return len;
}

uint32_t protocol_crc32value(uint32_t i)
{
	uint32_t j;
	uint32_t crc;
	crc = i;
	for (j = 8; j > 0; j--)
	{
		if (crc & 1)
		{
			crc = (crc >> 1) ^ CRC32_SEED_VAL;
		}
		else
		{
			crc >>= 1;
		}
	}
	return crc;
}

uint32_t protocol_calc32crc(uint8_t *p_buff, uint32_t len)
{
	if (len <= 0)
	{
		return 0;
	}
	uint32_t temp1;
	uint32_t temp2;
	uint32_t crc = 0;
	while (len-- != 0)
	{
		temp1 = (crc >> 8) & 0x00FFFFFFL;
		temp2 = protocol_crc32value(((uint32_t) crc ^ *p_buff++) & 0xff);
		crc = temp1 ^ temp2;
	}
	return crc;
}

