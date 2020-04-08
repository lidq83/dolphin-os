/*
 * uart.c
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#include <string.h>
#include <uart1dma.h>

void UART1_GPIO_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART1_Configuration(void)
{
	USART_InitTypeDef usart1_init_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	usart1_init_struct.USART_BaudRate = 921600;
	usart1_init_struct.USART_WordLength = USART_WordLength_8b;
	usart1_init_struct.USART_StopBits = USART_StopBits_1;
	usart1_init_struct.USART_Parity = USART_Parity_No;
	usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usart1_init_struct);
	USART_Cmd(USART1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

u8 sendbuf[1024];
u8 receivebuf[1024];

void _uart1_dma_configuration()
{
	DMA_InitTypeDef DMA_InitStructure;

	/* DMA1 Channel6 (triggered by USART1 Rx event) Config */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DMA1 Channel5 (triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);		// 初始化外设地址，相当于“哪家快递”
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)receivebuf;					// 内存地址，相当于几号柜
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//外设作为数据来源，即为收快递
	DMA_InitStructure.DMA_BufferSize = 1024;								// 缓存容量，即柜子大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// 外设地址不递增，即柜子对应的快递不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// 内存递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设字节宽度，即快递运输快件大小度量（按重量算，还是按体积算）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			// 内存字节宽度，即店主封装快递的度量(按重量，还是按体质进行封装)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// 正常模式，即满了就不在接收了，而不是循环存储
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					// 优先级很高，对应快递就是加急
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// 内存与外设通信，而非内存到内存
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							// 把参数初始化，即拟好与快递公司的协议

	DMA_Cmd(DMA1_Channel5, ENABLE); // 启动DMA，即与快递公司签订合同，正式生效

	/* DMA1 Channel4 (triggered by USART1 Tx event) Config */
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR); // 外设地址，串口1， 即发件的快递
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sendbuf;				// 发送内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;					// 外设为传送数据目的地，即发送数据，即快递是发件
	DMA_InitStructure.DMA_BufferSize = 0;								//发送长度为0，即未有快递需要发送
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);						//初始化

	USART_ITConfig(USART1, USART_IT_TC, ENABLE);					 // 使能串口发送完成中断
	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE); // 使能DMA串口发送和接受请求
}

static uint32_t i_flag = 0;

void USART1_IRQHandler(void)
{
	uint32_t temp = 0;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //空闲中断
	{
		//Uart1_Rx_Fifo.in = BSP_UART1_RX_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
		temp = USART1->SR; //软件序列清除IDLE位
		temp = USART1->DR; //先读USART_SR,然后读USART_DR
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
	else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET) //发送完成中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		DMA_Cmd(DMA1_Channel4, DISABLE);
		//OSSemPost(&Uart1TxWaitSem, OS_OPT_POST_NONE, &err);
	}
}

void uart1_init()
{
	UART1_GPIO_Configuration();
	UART1_Configuration();
	_uart1_dma_configuration();
}
