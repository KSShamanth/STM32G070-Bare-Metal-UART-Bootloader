/*
 * 004uart.c
 *
 *  Created on: 10-Aug-2026
 *      Author: apple
 */


#include "stm32g070xx.h"
#include <string.h>

uint8_t txbuffer[] = "This is the application";

int main(void){

	GPIO_Handle_t txpin, rxpin;
	memset(&txpin,0,sizeof(txpin));
	memset(&rxpin,0,sizeof(rxpin));

	UART_Handle_t uart2;
	memset(&uart2,0,sizeof(uart2));

	//txpin config
	txpin.pGPIOx = GPIOA;
	txpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN2;
	txpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	txpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	txpin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	txpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	txpin.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_PIN_AF1;

	//rxpin config
	rxpin.pGPIOx = GPIOA;
	rxpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN3;
	rxpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	rxpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	rxpin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	rxpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	rxpin.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_PIN_AF1;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&txpin);

	GPIO_Init(&rxpin);

	//uart config
	uart2.pUSARTx = USART2;
	uart2.UART_Config.UART_BaudRate = UART_BAUD_115200;
	uart2.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	uart2.UART_Config.UART_StopBits = UART_STOPBITS_1;
	uart2.UART_Config.UART_ParityControl = UART_PARITY_DI;
	uart2.UART_Config.UART_Mode = UART_MODE_TXRX;

	UART_Init(&uart2);

	UART_SendData(&uart2,txbuffer,sizeof(txbuffer) - 1);



	while(1);

	return 0;
}
