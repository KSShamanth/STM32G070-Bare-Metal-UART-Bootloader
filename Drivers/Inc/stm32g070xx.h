/*
 * stm32g070xx.h
 *
 *  Created on: 24-Jul-2026
 *      Author: apple
 */

#ifndef INC_STM32G070XX_H_
#define INC_STM32G070XX_H_

#include <stdint.h>

/********************* Processor specific details **************************/

#define NVIC_ISER				((volatile uint32_t*) 0xE000E100U)
#define NVIC_ICER				((volatile uint32_t*) 0xE000E180U)

#define SCB_BASEADDR			0xE000ED00U

#define NVIC_IPR_BASEADDR		((volatile uint32_t*) 0xE000E400U)

#define NO_PR_BITS_IMPLEMENTED  2U





/********************* Microcontroller specific details **************************/

#define HSI_CLOCK          		16000000U

#define MAINFLASH_BASEADDR 		0x08000000U
#define SRAM_BASEADDR  			0x20000000U
#define ROM_BASEADDR			0x1FFF0000U

#define PERIPH_BASEADDR			0x40000000U
#define APBPERIPH_BASEADDR      PERIPH_BASEADDR
#define AHBPERIPH_BASEADDR		0x40020000U
#define IOPORT_BASEADDR			0x50000000U

#define GPIOA_BASEADDR			(IOPORT_BASEADDR + 0x0000)
#define GPIOB_BASEADDR			(IOPORT_BASEADDR + 0x0400)
#define GPIOC_BASEADDR			(IOPORT_BASEADDR + 0x0800)
#define GPIOD_BASEADDR			(IOPORT_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR			(IOPORT_BASEADDR + 0x1000)
#define GPIOF_BASEADDR			(IOPORT_BASEADDR + 0x1400)



//peripherals hanging on APB bus

#define I2C1_BASEADDR			(APBPERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR			(APBPERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR			(APBPERIPH_BASEADDR + 0x8800)

#define SPI1_BASEADDR			(APBPERIPH_BASEADDR + 0x13000)
#define SPI2_BASEADDR			(APBPERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR			(APBPERIPH_BASEADDR + 0x3C00)

#define USART1_BASEADDR			(APBPERIPH_BASEADDR + 0x13800)
#define USART2_BASEADDR			(APBPERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR			(APBPERIPH_BASEADDR + 0x4800)
#define USART4_BASEADDR			(APBPERIPH_BASEADDR + 0x4C00)
#define USART5_BASEADDR			(APBPERIPH_BASEADDR + 0x5000)
#define USART6_BASEADDR			(APBPERIPH_BASEADDR + 0x13C00)

#define SYSCFG_BASEADDR			(APBPERIPH_BASEADDR + 0x10000)

//peripherals hanging on AHB bus

#define EXTI_BASEADDR			(AHBPERIPH_BASEADDR + 0x1800)
#define RCC_BASEADDR			(AHBPERIPH_BASEADDR + 0x1000)

#define FLASH_BASEADDR			(AHBPERIPH_BASEADDR + 0x2000)


/************* Peripheral register definition structures *************/


typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	volatile uint32_t BRR;
}GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t ICSCR;
	volatile uint32_t CFGR;
	volatile uint32_t PLL_CFGR;
	uint32_t 		  Reserved0;
	uint32_t 		  Reserved1;
	volatile uint32_t CIER;
	volatile uint32_t CIFR;
	volatile uint32_t CICR;
	volatile uint32_t IOPRSTR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t APBRSTR1;
	volatile uint32_t APBRSTR2;
	volatile uint32_t IOPENR;
	volatile uint32_t AHBENR;
	volatile uint32_t APBENR1;
	volatile uint32_t APBENR2;
	volatile uint32_t IOPSMENR;
	volatile uint32_t AHBSMENR;
	volatile uint32_t APBSMENR1;
	volatile uint32_t APBSMENR2;
	volatile uint32_t CCIPR;
	volatile uint32_t CCIPR2;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
}RCC_RegDef_t;

typedef struct
{
	volatile uint32_t RTSR1;
	volatile uint32_t FTSR1;
	volatile uint32_t SWIER1;
	volatile uint32_t RPR1;
	volatile uint32_t FPR1;
	uint32_t 		  Reserved0;
	uint32_t 		  Reserved1;
	uint32_t 		  Reserved2;
	uint32_t 		  Reserved3;
	uint32_t 		  Reserved4;
	uint32_t 		  Reserved5;
	uint32_t 		  Reserved6;
	uint32_t 		  Reserved7;
	uint32_t 		  Reserved8;
	uint32_t 		  Reserved9;
	uint32_t 		  Reserved10;
	uint32_t 		  Reserved11;
	uint32_t 		  Reserved12;
	uint32_t 		  Reserved13;
	uint32_t 		  Reserved14;
	uint32_t 		  Reserved15;
	uint32_t 		  Reserved16;
	uint32_t 		  Reserved17;
	uint32_t 		  Reserved18;
	volatile uint32_t EXTICR[4];
	uint32_t 		  Reserved19;
	uint32_t 		  Reserved20;
	uint32_t 		  Reserved21;
	uint32_t 		  Reserved22;
	volatile uint32_t IMR1;
	volatile uint32_t EMR1;
	uint32_t 		  Reserved23;
	uint32_t 		  Reserved24;
}EXTI_RegDef_t;

typedef struct
{
	volatile uint32_t CFGR1;
	uint32_t 		  Reserved0;
	uint32_t 		  Reserved1;
	uint32_t 		  Reserved2;
	uint32_t 		  Reserved3;
	uint32_t 		  Reserved4;
	uint32_t 		  Reserved5;
	uint32_t 		  Reserved6;
	volatile uint32_t CFGR2;
	volatile uint32_t ITLINE0;
	uint32_t 		  Reserved7;
	volatile uint32_t FPR1;
	volatile uint32_t ITLINE2;
	volatile uint32_t ITLINE3;
	volatile uint32_t ITLINE4;
	volatile uint32_t ITLINE5;
	volatile uint32_t ITLINE6;
	volatile uint32_t ITLINE7;
	volatile uint32_t ITLINE8;
	volatile uint32_t ITLINE9;
	volatile uint32_t ITLINE10;
	volatile uint32_t ITLINE11;
	volatile uint32_t ITLINE12;
	volatile uint32_t ITLINE13;
	volatile uint32_t ITLINE14;
	uint32_t 		  Reserved8;
	volatile uint32_t ITLINE16;
	volatile uint32_t ITLINE17;
	volatile uint32_t ITLINE18;
	volatile uint32_t ITLINE19;
	volatile uint32_t ITLINE20;
	volatile uint32_t ITLINE21;
	volatile uint32_t ITLINE22;
	volatile uint32_t ITLINE23;
	volatile uint32_t ITLINE24;
	volatile uint32_t ITLINE25;
	volatile uint32_t ITLINE26;
	volatile uint32_t ITLINE27;
	volatile uint32_t ITLINE28;
	volatile uint32_t ITLINE29;
	uint32_t 		  Reserved9;
	uint32_t 		  Reserved10;
	uint32_t 		  Reserved11;
}SYSCFG_RegDef_t;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t GPTR;
	volatile uint32_t RTOR;
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
	volatile uint32_t PRESC;

}USART_RegDef_t;

typedef struct
{
	volatile uint32_t CPUID;
	volatile uint32_t ICSR;
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;
	volatile uint32_t SCR;
	volatile uint32_t CCR;
	uint32_t 		  Reserved0;
	volatile uint32_t SHPR2;
	volatile uint32_t SHPR3;
	volatile uint32_t SHCSR;
	uint32_t 		  Reserved1;
	uint32_t 		  Reserved2;
	volatile uint32_t DFSR;

}SCB_RegDef_t;

typedef struct
{
	volatile uint32_t ACR;
	uint32_t 		  Reserved0;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t ECCR;
	uint32_t 		  Reserved1;
	volatile uint32_t OPTR;
	uint32_t 		  Reserved2;
	uint32_t 		  Reserved3;
	uint32_t 		  Reserved4;
	volatile uint32_t WRP1AR;
	volatile uint32_t WRP1BR;
	uint32_t 		  Reserved5;
	uint32_t 		  Reserved6;
	uint32_t 		  Reserved7;
	uint32_t 		  Reserved8;
	uint32_t 		  Reserved9;
	uint32_t 		  Reserved10;
	volatile uint32_t WRP2BR;
	volatile uint32_t DFSR;

}FLASH_RegDef_t;



/************* Peripheral definitions*************/

#define GPIOA 					((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 					((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 					((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 					((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 					((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 					((GPIO_RegDef_t*)GPIOF_BASEADDR)

#define RCC						((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI					((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG					((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define USART1					((USART_RegDef_t*)USART1_BASEADDR)
#define USART2					((USART_RegDef_t*)USART2_BASEADDR)
#define USART3					((USART_RegDef_t*)USART3_BASEADDR)
#define USART4					((USART_RegDef_t*)USART4_BASEADDR)
#define USART5					((USART_RegDef_t*)USART5_BASEADDR)
#define USART6					((USART_RegDef_t*)USART6_BASEADDR)

#define SCB						((SCB_RegDef_t*)SCB_BASEADDR)

#define FLASH 	                ((FLASH_RegDef_t*)FLASH_BASEADDR)


/* Clock enable macros for GPIOx peripherals */

#define GPIOA_PCLK_EN()		(RCC->IOPENR |= (1 << 0))
#define GPIOB_PCLK_EN()		(RCC->IOPENR |= (1 << 1))
#define GPIOC_PCLK_EN()		(RCC->IOPENR |= (1 << 2))
#define GPIOD_PCLK_EN()		(RCC->IOPENR |= (1 << 3))
#define GPIOE_PCLK_EN()		(RCC->IOPENR |= (1 << 4))
#define GPIOF_PCLK_EN()		(RCC->IOPENR |= (1 << 5))

/* Clock enable macros for I2Cx peripherals */

#define I2C1_PCLK_EN()		(RCC->APBENR1 |= (1 << 21))
#define I2C2_PCLK_EN()		(RCC->APBENR1 |= (1 << 22))
#define I2C3_PCLK_EN()		(RCC->APBENR1 |= (1 << 23))

/* Clock enable macros for SPIx peripherals */

#define SPI1_PCLK_EN()		(RCC->APBENR2 |= (1 << 12))
#define SPI2_PCLK_EN()		(RCC->APBENR1 |= (1 << 14))
#define SPI3_PCLK_EN()		(RCC->APBENR1 |= (1 << 15))

/* Clock enable macros for USARTx peripherals */

#define USART1_PCLK_EN()		(RCC->APBENR2 |= (1 << 14))
#define USART2_PCLK_EN()		(RCC->APBENR1 |= (1 << 17))
#define USART3_PCLK_EN()		(RCC->APBENR1 |= (1 << 18))
#define USART4_PCLK_EN()		(RCC->APBENR1 |= (1 << 19))
#define USART5_PCLK_EN()		(RCC->APBENR1 |= (1 << 8))
#define USART6_PCLK_EN()		(RCC->APBENR1 |= (1 << 9))

/* Clock enable macros for SYSCFG peripherals */

#define SYSCFG_PCLK_EN()		(RCC->APBENR2 |= (1 << 0))

/* Clock disable macros for GPIOx peripherals */

#define GPIOA_PCLK_DI()		(RCC->IOPENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()		(RCC->IOPENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()		(RCC->IOPENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()		(RCC->IOPENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()		(RCC->IOPENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()		(RCC->IOPENR &= ~(1 << 5))

/* Clock disable macros for I2Cx peripherals */

#define I2C1_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 21))
#define I2C2_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 22))
#define I2C3_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 23))

/* Clock disable macros for SPIx peripherals */

#define SPI1_PCLK_DI()		(RCC->APBENR2 &= ~(1 << 12))
#define SPI2_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 14))
#define SPI3_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 15))

/* Clock disable macros for USARTx peripherals */

#define USART1_PCLK_DI()		(RCC->APBENR2 &= ~(1 << 14))
#define USART2_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 17))
#define USART3_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 18))
#define USART4_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 19))
#define USART5_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 8))
#define USART6_PCLK_DI()		(RCC->APBENR1 &= ~(1 << 9))

/* Clock disable macros for SYSCFG peripherals */

#define SYSCFG_PCLK_DI()		(RCC->APBENR2 &= ~(1 << 0))

/* macros to reset gpiox peripherals */

#define GPIOA_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 0));		(RCC->IOPRSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 1));		(RCC->IOPRSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 2));		(RCC->IOPRSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 3));		(RCC->IOPRSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 4));		(RCC->IOPRSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()			do{ (RCC->IOPRSTR |= (1 << 5));		(RCC->IOPRSTR &= ~(1 << 5)); } while(0)

/* returns port code for given GPIOx base address */
#define GPIO_BASEADDR_TO_CODE(x)  ( (x == GPIOA) ? 00 :\
									(x == GPIOB) ? 01 :\
									(x == GPIOC) ? 02 :\
									(x == GPIOD) ? 03 :\
									(x == GPIOF) ? 05 :0 )

/* macros to reset uart peripherals */

#define USART1_REG_RESET()			do{ (RCC->APBRSTR2 |= (1 << 14));    (RCC->APBRSTR2 &= ~(1 << 14)); } while(0);
#define USART2_REG_RESET()			do{ (RCC->APBRSTR1 |= (1 << 17));    (RCC->APBRSTR1 &= ~(1 << 17)); } while(0);
#define USART3_REG_RESET()			do{ (RCC->APBRSTR1 |= (1 << 18));    (RCC->APBRSTR1 &= ~(1 << 18)); } while(0);
#define USART4_REG_RESET()			do{ (RCC->APBRSTR1 |= (1 << 19));    (RCC->APBRSTR1 &= ~(1 << 19)); } while(0);
#define USART5_REG_RESET()			do{ (RCC->APBRSTR1 |= (1 << 8));     (RCC->APBRSTR1 &= ~(1 << 8));  } while(0);
#define USART6_REG_RESET()			do{ (RCC->APBRSTR1 |= (1 << 9));     (RCC->APBRSTR1 &= ~(1 << 9));  } while(0);

//some generic macros

#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_PIN_SET 	SET
#define GPIO_PIN_RESET 	RESET

//macro for irq

#define IRQ_NO_EXTI0_1 			5
#define IRQ_NO_EXTI2_3 			6
#define IRQ_NO_EXTI4_15 		7

#define NVIC_IRQ_PRI0           0
#define NVIC_IRQ_PRI1           1
#define NVIC_IRQ_PRI2           2
#define NVIC_IRQ_PRI3           3


#include "stm32g070xx_gpio_driver.h"

#include "stm32g070xx_uart_driver.h"


#endif /* INC_STM32G070XX_H_ */
