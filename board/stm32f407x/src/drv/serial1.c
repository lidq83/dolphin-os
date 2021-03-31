#include <serial1.h>

#define RX_RING_BUFFER1 (RX_BUFFER1_SIZE + 1) // +1 added by MS
#define TX_RING_BUFFER1 (TX_BUFFER1_SIZE + 1) // +1 added by MS

static uint8_t serial1_rx_buffer[RX_RING_BUFFER1];
static uint8_t serial1_rx_buffer_head = 0;
volatile uint8_t serial1_rx_buffer_tail = 0;

static uint8_t serial1_tx_buffer[TX_RING_BUFFER1];
static uint8_t serial1_tx_buffer_head = 0;
volatile uint8_t serial1_tx_buffer_tail = 0;

void* serial1_addr(void)
{
	return serial1_rx_buffer;
}

static void storeHandleDataIn(uint8_t data);

// Returns the number of bytes available in the RX serial buffer.
uint8_t serial1_get_rx_buffer_available()
{
	int16_t freeSpace;
	freeSpace = ((int16_t)serial1_rx_buffer_tail) - ((int16_t)serial1_rx_buffer_head);
	if (freeSpace <= 0)
	{
		freeSpace += RX_RING_BUFFER1;
	}
	return ((uint8_t)freeSpace - 1);
}

// Returns the number of bytes used in the RX serial buffer.
// NOTE: Deprecated. Not used unless classic status reports are enabled in config.h.
uint8_t serial1_get_rx_buffer_count()
{
	uint8_t rtail = serial1_rx_buffer_tail; // Copy to limit multiple calls to volatile
	if (serial1_rx_buffer_head >= rtail)
	{
		return (serial1_rx_buffer_head - rtail);
	}
	return (RX_BUFFER1_SIZE - (rtail - serial1_rx_buffer_head));
}

void serial1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); // USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // USART1_RX

	USART_InitStructure.USART_BaudRate = SERIAL1_BAUTRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE);
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

// Writes one byte to the TX serial buffer. Called by main program.
void serial1_write(uint8_t data)
{
	// Calculate next head
	uint8_t next_head = serial1_tx_buffer_head + 1;
	if (next_head >= TX_RING_BUFFER1)
	{
		next_head = 0;
	}

	// Wait until there is space in the buffer
	while (next_head == serial1_tx_buffer_tail)
	{
		// TODO: Restructure st_prep_buffer() calls to be executed here during a long print.
		//if (sys_rt_exec_state & EXEC_RESET) { return; } // put as comment by MS because we loose some characters Only check for abort to avoid an endless loop.
	}

	// Store data and advance head
	serial1_tx_buffer[serial1_tx_buffer_head] = data;
	serial1_tx_buffer_head = next_head;

	USART1->CR1 |= USART_FLAG_TXE; // added by MS		          // enable TX interrupt// it seems it is safier to do it in all cases
}

// Fetches the first byte in the serial read buffer. Called by main program.
int serial1_read(uint8_t *ch)
{
	uint8_t tail = serial1_rx_buffer_tail; // Temporary serial1_rx_buffer_tail (to optimize for volatile)
	if (serial1_rx_buffer_head == tail)
	{
		return 0;
	}
	else
	{
		uint8_t data = serial1_rx_buffer[tail];

		tail++;
		//if (tail == RX_RING_BUFFER1) { tail = 0; }   // removed by MS it was
		if (tail >= RX_RING_BUFFER1)
		{
			tail = 0;
		} // added by MS
		serial1_rx_buffer_tail = tail;
		*ch = data;
		return 1;
	}
}

// store one received byte into Rx buffer or handle if it is a realtime command
void storeHandleDataIn(uint8_t data)
{
	uint8_t next_head;
	// Pick off realtime command characters directly from the serial stream. These characters are
	// not passed into the main buffer, but these set system state flag bits for realtime execution.
	next_head = serial1_rx_buffer_head + 1;
	//if (next_head == RX_RING_BUFFER1) { next_head = 0; }
	if (next_head >= RX_RING_BUFFER1)
	{
		next_head = 0;
	}

	// Write data to buffer unless it is full. Take care: if buffer is full, then car is lost
	if (next_head != serial1_rx_buffer_tail)
	{
		serial1_rx_buffer[serial1_rx_buffer_head] = data;
		serial1_rx_buffer_head = next_head;
	}
	else
	{				 // added by MS to get a breakpunt
		next_head++; //just do nothing but allow a break punt
		next_head--;
	}
}

/*----------------------------------------------------------------------------
 USART1_IRQHandler
 Handles USART1 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	//volatile unsigned int IIR;
	uint8_t data;
	uint8_t tail;

	//IIR = USART1->SR;
	if (USART1->SR & USART_FLAG_RXNE) // changed by MS : it was IIR & USART_FLAG_RXNE
	{								  // read interrupt
		data = USART1->DR & 0x1FF;
		storeHandleDataIn(data);
		//USART1->SR = ~USART_FLAG_RXNE;	          // clear interrupt ; changed by MS, it is normally cleared automatically by reading the DR register
	}

	if (USART1->SR & USART_FLAG_TXE)
	{
		// changed by MS : it was IIR & USART_FLAG_TXE
		tail = serial1_tx_buffer_tail;
		if (tail != serial1_tx_buffer_head)
		{										  // if there is at least one byte to send; take it from the buffer and
			USART1->DR = serial1_tx_buffer[tail]; // this clear the interrupt.
			tail++;
			if (tail >= TX_RING_BUFFER1)
			{
				tail = 0;
			}
			serial1_tx_buffer_tail = tail;
		}
		else
		{
			USART1->CR1 &= ~USART_FLAG_TXE; // disable TX interrupt if nothing to send
			USART1->SR = ~USART_FLAG_TXE;	// clear interrupt
		}

	} // end of sending
} // end of interrupt

void serial1_reset_read_buffer()
{
	serial1_rx_buffer_tail = serial1_rx_buffer_head;
}
