/*
 * stm32g070xx_gpio_driver.c
 *
 * Created on: 24-Jul-2026
 * Author: apple
 */

#include "stm32g070xx_gpio_driver.h"
#include <stdint.h>


/* GPIO clock control */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
	}
}


/* GPIO initialization */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;
	uint8_t pin = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

	/* Configure pin mode */

	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pin));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3U << (2 * pin));
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else
	{
		pGPIOHandle->pGPIOx->MODER &= ~(0x3U << (2 * pin));

		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_FT)
		{
			EXTI->FTSR1 |= (1U << pin);
			EXTI->RTSR1 &= ~(1U << pin);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_RT)
		{
			EXTI->RTSR1 |= (1U << pin);
			EXTI->FTSR1 &= ~(1U << pin);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_RFT)
		{
			EXTI->RTSR1 |= (1U << pin);
			EXTI->FTSR1 |= (1U << pin);
		}

		uint8_t temp1 = pin / 4;
		uint8_t temp2 = pin % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		EXTI->EXTICR[temp1] &= ~(0xFFU << (temp2 * 8));
		EXTI->EXTICR[temp1] |= (portcode << (temp2 * 8));

		EXTI->IMR1 |= (1U << pin);
	}

	/* Configure pin speed */

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pin));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3U << (2 * pin));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	/* Configure pull-up/pull-down */

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pin));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3U << (2 * pin));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	/* Configure output type */

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pin);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1U << pin);
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	/* Configure alternate function */

	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		if (pin <= GPIO_PIN7)
		{
			pGPIOHandle->pGPIOx->AFRL &= ~(0xFU << (4 * pin));
			pGPIOHandle->pGPIOx->AFRL |=
					(pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pin));
		}
		else
		{
			pGPIOHandle->pGPIOx->AFRH &= ~(0xFU << (4 * (pin % 8)));
			pGPIOHandle->pGPIOx->AFRH |=
					(pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * (pin % 8)));
		}
	}
}


/* GPIO de-initialization */

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
}


/* GPIO input functions */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value = (pGPIOx->IDR >> PinNumber) & 1U;

	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value = pGPIOx->IDR;

	return value;
}


/* GPIO output functions */

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if (value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1U << PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1U << PinNumber);
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
	pGPIOx->ODR = value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1U << PinNumber);
}


/* GPIO interrupt configuration */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		*NVIC_ISER |= (1U << IRQNumber);
	}
	else
	{
		*NVIC_ICER |= (1U << IRQNumber);
	}
}


/* GPIO interrupt priority */

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amt = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_IPR_BASEADDR + iprx) |= IRQPriority << shift_amt;
}


/* GPIO interrupt handling */

void GPIO_IRQHandling(uint8_t PinNumber)
{
	if (EXTI->RPR1 & (1U << PinNumber))
	{
		EXTI->RPR1 = (1U << PinNumber);
	}

	if (EXTI->FPR1 & (1U << PinNumber))
	{
		EXTI->FPR1 = (1U << PinNumber);
	}
}
