#ifndef INC_STM32G070XX_UART_DRIVER_H_
#define INC_STM32G070XX_UART_DRIVER_H_

#include "stm32g070xx.h"


/* UART configuration */

typedef struct
{
	uint32_t UART_BaudRate;
	uint8_t UART_WordLength;
	uint8_t UART_StopBits;
	uint8_t UART_ParityControl;
	uint8_t UART_Mode;

} UART_Config_t;


/* UART handle */

typedef struct
{
	USART_RegDef_t *pUSARTx;
	UART_Config_t UART_Config;

} UART_Handle_t;


/* UART baud rates */

#define UART_BAUD_9600			9600
#define UART_BAUD_19200			19200
#define UART_BAUD_38400			38400
#define UART_BAUD_57600			57600
#define UART_BAUD_115200		115200


/* UART word lengths */

#define UART_WORDLEN_7BITS		0
#define UART_WORDLEN_8BITS		1
#define UART_WORDLEN_9BITS		2


/* UART stop bits */

#define UART_STOPBITS_1			0
#define UART_STOPBITS_2			2


/* UART parity */

#define UART_PARITY_DI			0
#define UART_PARITY_EVEN		1
#define UART_PARITY_ODD			2


/* UART modes */

#define UART_MODE_TX			0
#define UART_MODE_RX			1
#define UART_MODE_TXRX			2


/* UART APIs */

uint32_t UART_ComputeBRR(uint32_t pclk, uint32_t baud);

void UART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

void UART_Init(UART_Handle_t *pUARTHandle);

void UART_DeInit(USART_RegDef_t *pUSARTx);

void UART_SendData(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len);

void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);

void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);

void UART_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

void UART_IRQHandling(UART_Handle_t *pUARTHandle);


#endif /* INC_STM32G070XX_UART_DRIVER_H_ */
