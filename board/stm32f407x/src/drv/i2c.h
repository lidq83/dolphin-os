#ifndef __I2C_H
#define __I2C_H

#include <typedef.h>

#define I2C2_PERIPH RCC_APB1Periph_I2C2
#define I2C2_RCC_Port RCC_AHB1Periph_GPIOB

#define I2C2_GPIO_PORT GPIOB
#define I2C2_SCL_Pin GPIO_Pin_10
#define I2C2_SDA_Pin GPIO_Pin_11

#define I2C2_SCL_PIN_SOURCE GPIO_PinSource10
#define I2C2_SDA_PIN_SOURCE GPIO_PinSource11

#define I2C2_DEFAULT_ADDRESS (0x3C)
#define I2C2_SPEED 100000
#define I2C2_I2Cx I2C2

void i2c2_init();
void i2c2_byte_write(u8 slaveAddr, u8* pBuffer, u8 WriteAddr);
void i2c2_buffer_read(u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);

#endif
