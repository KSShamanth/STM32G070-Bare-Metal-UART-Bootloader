
#include "crc.h"
#include "stm32g070xx.h"
#include <string.h>
#include "bootloader.h"
#include "flash_driver.h"



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

	GPIO_Init(&rxpin);

	GPIO_Init(&txpin);

	//uart config
	uart2.pUSARTx = USART2;
	uart2.UART_Config.UART_BaudRate = UART_BAUD_115200;
	uart2.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	uart2.UART_Config.UART_StopBits = UART_STOPBITS_1;
	uart2.UART_Config.UART_ParityControl = UART_PARITY_DI;
	uart2.UART_Config.UART_Mode = UART_MODE_TXRX;

	UART_Init(&uart2);

	Bootloader_Run(&uart2);




}
