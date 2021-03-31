#include <i2c.h>

/**
 * @brief  Initializes the I2C peripheral used to drive the HMC5883L
 * @param  None
 * @retval None
 */
void i2c2_init()
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable I2C and GPIO clocks */
	RCC_APB1PeriphClockCmd(I2C2_PERIPH, ENABLE);
	RCC_AHB1PeriphClockCmd(I2C2_RCC_Port, ENABLE);

	/* Configure I2C pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin = I2C2_SCL_Pin | I2C2_SDA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SCL_PIN_SOURCE, GPIO_AF_I2C2); // TODO Add or remove defines
	GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SDA_PIN_SOURCE, GPIO_AF_I2C2);

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_DEFAULT_ADDRESS; // HMC5883L 7-bit adress = 0x1E;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C2_SPEED;

	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C2_I2Cx, &I2C_InitStructure);
	I2C_Cmd(I2C2_I2Cx, ENABLE);
}

/**
 * @brief  Writes one byte to the  HMC5883L.
 * @param  slaveAddr : slave address HMC5883L_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the HMC5883L.
 * @param  WriteAddr : address of the register in which the data will be written
 * @retval None
 */
void i2c2_byte_write(u8 slaveAddr, u8* pBuffer, u8 WriteAddr)
{
	// ENTR_CRT_SECTION();

	/* Send START condition */
	I2C_GenerateSTART(I2C2_I2Cx, ENABLE);

	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/* Send HMC5883 address for write */
	I2C_Send7bitAddress(I2C2_I2Cx, slaveAddr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	/* Send the HMC5883L internal address to write to */
	I2C_SendData(I2C2_I2Cx, WriteAddr);

	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	/* Send the byte to be written */
	I2C_SendData(I2C2_I2Cx, *pBuffer);

	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	/* Send STOP condition */
	I2C_GenerateSTOP(I2C2_I2Cx, ENABLE);
	// EXT_CRT_SECTION();
}

/**
 * @brief  Reads a block of data from the HMC5883L.
 * @param  slaveAddr  : slave address HMC5883L_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer that receives the data read from the HMC5883L.
 * @param  ReadAddr : HMC5883L's internal address to read from.
 * @param  NumByteToRead : number of bytes to read from the HMC5883L ( NumByteToRead >1  only for the Magnetometer reading).
 * @retval None
 */
void i2c2_buffer_read(u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	// ENTR_CRT_SECTION();

	/* While the bus is busy */
	while (I2C_GetFlagStatus(I2C2_I2Cx, I2C_FLAG_BUSY))
		;

	/* Send START condition */
	I2C_GenerateSTART(I2C2_I2Cx, ENABLE);

	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/* Send HMC5883L_Magn address for write */ // Send HMC5883L address for write
	I2C_Send7bitAddress(I2C2_I2Cx, slaveAddr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C2_I2Cx, ENABLE);

	/* Send the HMC5883L's internal address to write to */
	I2C_SendData(I2C2_I2Cx, ReadAddr);

	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	/* Send STRAT condition a second time */
	I2C_GenerateSTART(I2C2_I2Cx, ENABLE);

	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;

	/* Send HMC5883L address for read */
	I2C_Send7bitAddress(I2C2_I2Cx, slaveAddr, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;

	/* While there is data to be read */
	while (NumByteToRead)
	{
		if (NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C2_I2Cx, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C2_I2Cx, ENABLE);
		}

		/* Test on EV7 and clear it */
		if (I2C_CheckEvent(I2C2_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			/* Read a byte from the HMC5883L */
			*pBuffer = I2C_ReceiveData(I2C2_I2Cx);

			/* Point to the next location where the byte read will be saved */
			pBuffer++;

			/* Decrement the read bytes counter */
			NumByteToRead--;
		}
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C2_I2Cx, ENABLE);
	// EXT_CRT_SECTION();
}