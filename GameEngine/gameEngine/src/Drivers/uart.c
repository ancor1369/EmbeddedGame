/*
 * uart.c
 *
 *  Created on: Nov. 16, 2019
 *      Author: Andres Rogelio Cordoba
 *
 *      Vast majority of the code was taken from the example
 *      provided by the maker to initialize the UART interface.
 *
 *
 */

#include "../Drivers/uart.h"

RINGBUFF_T txring, rxring;
uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */
void UART_HANDLER(void)
{
	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(UART_SELECTION, &rxring, &txring);
}

char rx_buff[UART_RRB_SIZE];
char tx_buff[UART_SRB_SIZE];


/**
 * @brief LED task
 * @param pvParameters
 */
void vTaskUART(void *pvParameters)
{
	int bytes;

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(UART_SELECTION);
	Chip_UART_SetBaud(UART_SELECTION, 115200);
	Chip_UART_ConfigData(UART_SELECTION, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(UART_SELECTION, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(UART_SELECTION);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Reset and enable FIFOs, FIFO trigger level 3 (14 chars) */
	Chip_UART_SetupFIFOS(UART_SELECTION, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
							UART_FCR_TX_RS | UART_FCR_TRG_LEV3));

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(UART_SELECTION, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(IRQ_SELECTION, 1);
	NVIC_EnableIRQ(IRQ_SELECTION);

	/* Send initial message */
	char buf[] = "Initialization of the part interface active\r\n";
	Chip_UART_SendRB(UART_SELECTION, &txring, buf, sizeof(buf) - 1);

	while (1)
	{
		Board_LED_Set(2,false);
		bytes = Chip_UART_ReadRB(UART_SELECTION, &rxring, &rx_buff, sizeof(rx_buff));
			if (bytes > 0)
			{
				Chip_UART_SendRB(UART_SELECTION, &txring, &rx_buff, sizeof(rx_buff));
				for(int i = 0; i<UART_RRB_SIZE; i++)
				{
					rx_buff[i]=0;
				}
			}
		}
		Board_LED_Set(2,true);
		vTaskDelay(configTICK_RATE_HZ*5);
	}

