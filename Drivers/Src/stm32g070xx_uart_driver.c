#include "stm32g070xx_uart_driver.h"
#include <stdint.h>


/* UART clock control */

void UART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pUSARTx == USART1)
		{
			USART1_PCLK_EN();
		}
		else if (pUSARTx == USART2)
		{
			USART2_PCLK_EN();
		}
		else if (pUSARTx == USART3)
		{
			USART3_PCLK_EN();
		}
		else if (pUSARTx == USART4)
		{
			USART4_PCLK_EN();
		}
		else if (pUSARTx == USART5)
		{
			USART5_PCLK_EN();
		}
		else if (pUSARTx == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		if (pUSARTx == USART1)
		{
			USART1_PCLK_DI();
		}
		else if (pUSARTx == USART2)
		{
			USART2_PCLK_DI();
		}
		else if (pUSARTx == USART3)
		{
			USART3_PCLK_DI();
		}
		else if (pUSARTx == USART4)
		{
			USART4_PCLK_DI();
		}
		else if (pUSARTx == USART5)
		{
			USART5_PCLK_DI();
		}
		else if (pUSARTx == USART6)
		{
			USART6_PCLK_DI();
		}
	}
}


/* Calculate UART BRR */

uint32_t UART_ComputeBRR(uint32_t pclk, uint32_t baud)
{
	if (baud == 0U)
	{
		return 0U;
	}

	return (pclk + (baud / 2U)) / baud;
}


/* UART initialization */

void UART_Init(UART_Handle_t *pUARTHandle)
{
	UART_PeriClockControl(pUARTHandle->pUSARTx, ENABLE);

	uint32_t temp = 0;

	/* Configure baud rate */

	temp = UART_ComputeBRR(
			HSI_CLOCK,
			pUARTHandle->UART_Config.UART_BaudRate);

	pUARTHandle->pUSARTx->BRR = temp;


	/* Configure word length */

	if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_7BITS)
	{
		pUARTHandle->pUSARTx->CR1 |= (1U << 28);
		pUARTHandle->pUSARTx->CR1 &= ~(1U << 12);
	}
	else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_8BITS)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(1U << 28);
		pUARTHandle->pUSARTx->CR1 &= ~(1U << 12);
	}
	else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(1U << 28);
		pUARTHandle->pUSARTx->CR1 |= (1U << 12);
	}


	/* Configure stop bits */

	if (pUARTHandle->UART_Config.UART_StopBits == UART_STOPBITS_1)
	{
		pUARTHandle->pUSARTx->CR2 &= ~(3U << 12);
	}
	else if (pUARTHandle->UART_Config.UART_StopBits == UART_STOPBITS_2)
	{
		pUARTHandle->pUSARTx->CR2 &= ~(3U << 12);
		pUARTHandle->pUSARTx->CR2 |= (2U << 12);
	}


	/* Configure parity */

	if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(1U << 10);
	}
	else
	{
		pUARTHandle->pUSARTx->CR1 |= (1U << 10);

		if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EVEN)
		{
			pUARTHandle->pUSARTx->CR1 &= ~(1U << 9);
		}
		else
		{
			pUARTHandle->pUSARTx->CR1 |= (1U << 9);
		}
	}


	/* Configure UART mode */

	if (pUARTHandle->UART_Config.UART_Mode == UART_MODE_TX)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(3U << 2);
		pUARTHandle->pUSARTx->CR1 |= (1U << 3);
	}
	else if (pUARTHandle->UART_Config.UART_Mode == UART_MODE_RX)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(3U << 2);
		pUARTHandle->pUSARTx->CR1 |= (1U << 2);
	}
	else if (pUARTHandle->UART_Config.UART_Mode == UART_MODE_TXRX)
	{
		pUARTHandle->pUSARTx->CR1 &= ~(3U << 2);
		pUARTHandle->pUSARTx->CR1 |= (3U << 2);
	}


	/* Enable UART */

	pUARTHandle->pUSARTx->CR1 |= (1U << 0);
}


/* UART de-initialization */

void UART_DeInit(USART_RegDef_t *pUSARTx)
{
	if (pUSARTx == USART1)
	{
		USART1_REG_RESET();
	}
	else if (pUSARTx == USART2)
	{
		USART2_REG_RESET();
	}
	else if (pUSARTx == USART3)
	{
		USART3_REG_RESET();
	}
	else if (pUSARTx == USART4)
	{
		USART4_REG_RESET();
	}
	else if (pUSARTx == USART5)
	{
		USART5_REG_RESET();
	}
	else if (pUSARTx == USART6)
	{
		USART6_REG_RESET();
	}
}


/* UART transmit */

void UART_SendData(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	while (Len > 0U)
	{
		/* Wait for transmit buffer */

		while (!(pUARTHandle->pUSARTx->ISR & (1U << 7)))
		{
		}


		/* 9-bit data */

		if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			uint16_t *pdata = (uint16_t *)pTxBuffer;

			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				pUARTHandle->pUSARTx->TDR = (*pdata & 0x01FFU);
			}
			else
			{
				pUARTHandle->pUSARTx->TDR = (*pdata & 0x00FFU);
			}

			pdata++;
			pTxBuffer = (uint8_t *)pdata;
		}


		/* 8-bit data */

		else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_8BITS)
		{
			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				pUARTHandle->pUSARTx->TDR = *pTxBuffer;
			}
			else
			{
				pUARTHandle->pUSARTx->TDR = (*pTxBuffer & 0x7FU);
			}

			pTxBuffer++;
		}


		/* 7-bit data */

		else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_7BITS)
		{
			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				pUARTHandle->pUSARTx->TDR = (*pTxBuffer & 0x7FU);
			}
			else
			{
				pUARTHandle->pUSARTx->TDR = (*pTxBuffer & 0x3FU);
			}

			pTxBuffer++;
		}

		Len--;
	}


	/* Wait for transmission complete */

	while (!(pUARTHandle->pUSARTx->ISR & (1U << 6)))
	{
	}
}


/* UART receive */

void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len > 0U)
	{
		/* Wait for received data */

		while (!(pUARTHandle->pUSARTx->ISR & (1U << 5)))
		{
		}


		/* 9-bit data */

		if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			uint16_t *pdata = (uint16_t *)pRxBuffer;

			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				*pdata = (uint16_t)(pUARTHandle->pUSARTx->RDR & 0x01FFU);
			}
			else
			{
				*pdata = (uint16_t)(pUARTHandle->pUSARTx->RDR & 0x00FFU);
			}

			pdata++;
			pRxBuffer = (uint8_t *)pdata;
		}


		/* 8-bit data */

		else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_8BITS)
		{
			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				*pRxBuffer = pUARTHandle->pUSARTx->RDR & 0x0FFU;
			}
			else
			{
				*pRxBuffer = pUARTHandle->pUSARTx->RDR & 0x07FU;
			}

			pRxBuffer++;
		}


		/* 7-bit data */

		else if (pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_7BITS)
		{
			if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DI)
			{
				*pRxBuffer = pUARTHandle->pUSARTx->RDR & 0x07FU;
			}
			else
			{
				*pRxBuffer = pUARTHandle->pUSARTx->RDR & 0x03FU;
			}

			pRxBuffer++;
		}

		Len--;
	}
}
