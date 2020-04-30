#include <typedef.h>
#include <serial2.h>

#define RX_RING_BUFFER2 (RX_BUFFER2_SIZE + 1) // +1 added by MS
#define TX_RING_BUFFER2 (TX_BUFFER2_SIZE + 1) // +1 added by MS

uint8_t serial2_rx_buffer[RX_RING_BUFFER2];
uint8_t serial2_rx_buffer_head = 0;
volatile uint8_t serial2_rx_buffer_tail = 0;

uint8_t serial2_tx_buffer[TX_RING_BUFFER2];
uint8_t serial2_tx_buffer_head = 0;
volatile uint8_t serial2_tx_buffer_tail = 0;

// Returns the number of bytes available in the RX serial buffer.
uint8_t serial2_get_rx_buffer_available()
{
	int16_t freeSpace;
	freeSpace = ((int16_t)serial2_rx_buffer_tail) - ((int16_t)serial2_rx_buffer_head);
	if (freeSpace <= 0)
	{
		freeSpace += RX_RING_BUFFER2;
	}
	return ((uint8_t)freeSpace - 1);
}

// Returns the number of bytes used in the RX serial buffer.
// NOTE: Deprecated. Not used unless classic status reports are enabled in config.h.
uint8_t serial2_get_rx_buffer_count()
{
	uint8_t rtail = serial2_rx_buffer_tail; // Copy to limit multiple calls to volatile
	if (serial2_rx_buffer_head >= rtail)
	{
		return (serial2_rx_buffer_head - rtail);
	}
	return (RX_BUFFER2_SIZE - (rtail - serial2_rx_buffer_head));
}

void serial2_init()
{
}

// Writes one byte to the TX serial buffer. Called by main program.
void serial2_write(uint8_t data)
{
	// Calculate next head
	uint8_t next_head = serial2_tx_buffer_head + 1;
	if (next_head >= TX_RING_BUFFER2)
	{
		next_head = 0;
	}

	// Wait until there is space in the buffer
	while (next_head == serial2_tx_buffer_tail)
	{
		// TODO: Restructure st_prep_buffer() calls to be executed here during a long print.
		//if (sys_rt_exec_state & EXEC_RESET) { return; } // put as comment by MS because we loose some characters Only check for abort to avoid an endless loop.
	}

	// Store data and advance head
	serial2_tx_buffer[serial2_tx_buffer_head] = data;
	serial2_tx_buffer_head = next_head;

	USART2->CR1 |= USART_FLAG_TXE; // added by MS		          // enable TX interrupt// it seems it is safier to do it in all cases
}

// Fetches the first byte in the serial read buffer. Called by main program.
int serial2_read(uint8_t *ch)
{
	uint8_t tail = serial2_rx_buffer_tail; // Temporary serial2_rx_buffer_tail (to optimize for volatile)
	if (serial2_rx_buffer_head == tail)
	{
		return 0;
	}
	else
	{
		uint8_t data = serial2_rx_buffer[tail];

		tail++;
		//if (tail == RX_RING_BUFFER2) { tail = 0; }   // removed by MS it was
		if (tail >= RX_RING_BUFFER2)
		{
			tail = 0;
		} // added by MS
		serial2_rx_buffer_tail = tail;
		*ch = data;
		return 1;
	}
}

// store one received byte into Rx buffer or handle if it is a realtime command
void storeHandleDataIn2(uint8_t data)
{
	uint8_t next_head;
	// Pick off realtime command characters directly from the serial stream. These characters are
	// not passed into the main buffer, but these set system state flag bits for realtime execution.
	next_head = serial2_rx_buffer_head + 1;
	//if (next_head == RX_RING_BUFFER2) { next_head = 0; }
	if (next_head >= RX_RING_BUFFER2)
	{
		next_head = 0;
	}

	// Write data to buffer unless it is full. Take care: if buffer is full, then car is lost
	if (next_head != serial2_rx_buffer_tail)
	{
		serial2_rx_buffer[serial2_rx_buffer_head] = data;
		serial2_rx_buffer_head = next_head;
	}
	else
	{				 // added by MS to get a breakpunt
		next_head++; //just do nothing but allow a break punt
		next_head--;
	}
}

/*----------------------------------------------------------------------------
 USART2_IRQHandler
 Handles USART2 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler(void)
{
	//volatile unsigned int IIR;
	uint8_t data;
	uint8_t tail;

	//IIR = USART2->SR;
	if (USART2->SR & USART_FLAG_RXNE) // changed by MS : it was IIR & USART_FLAG_RXNE
	{								  // read interrupt
		data = USART2->DR & 0x1FF;
		storeHandleDataIn2(data);
		//USART2->SR = ~USART_FLAG_RXNE;	          // clear interrupt ; changed by MS, it is normally cleared automatically by reading the DR register
	}

	if (USART2->SR & USART_FLAG_TXE)
	{
		// changed by MS : it was IIR & USART_FLAG_TXE
		tail = serial2_tx_buffer_tail;
		if (tail != serial2_tx_buffer_head)
		{										 // if there is at least one byte to send; take it from the buffer and
			USART2->DR = serial2_tx_buffer[tail]; // this clear the interrupt.
			tail++;
			if (tail >= TX_RING_BUFFER2)
			{
				tail = 0;
			}
			serial2_tx_buffer_tail = tail;
		}
		else
		{
			USART2->CR1 &= ~USART_FLAG_TXE; // disable TX interrupt if nothing to send
			USART2->SR = ~USART_FLAG_TXE;	// clear interrupt
		}

	} // end of sending
} // end of interrupt

void serial2_reset_read_buffer()
{
	serial2_rx_buffer_tail = serial2_rx_buffer_head;
}
