#ifndef INC_STM32G070XX_GPIO_DRIVER_H_
#define INC_STM32G070XX_GPIO_DRIVER_H_

#include "stm32g070xx.h"


/* GPIO pin configuration */

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;

} GPIO_PinConfig_t;


/* GPIO handle */

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;

} GPIO_Handle_t;


/* GPIO pin numbers */

#define GPIO_PIN0				0
#define GPIO_PIN1				1
#define GPIO_PIN2				2
#define GPIO_PIN3				3
#define GPIO_PIN4				4
#define GPIO_PIN5				5
#define GPIO_PIN6				6
#define GPIO_PIN7				7
#define GPIO_PIN8				8
#define GPIO_PIN9				9
#define GPIO_PIN10				10
#define GPIO_PIN11				11
#define GPIO_PIN12				12
#define GPIO_PIN13				13
#define GPIO_PIN14				14
#define GPIO_PIN15				15


/* GPIO pin modes */

#define GPIO_MODE_IN			0
#define GPIO_MODE_OUT			1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOG		3
#define GPIO_MODE_IT_FT			4
#define GPIO_MODE_IT_RT			5
#define GPIO_MODE_IT_RFT		6


/* GPIO output types */

#define GPIO_OP_TYPE_PP			0
#define GPIO_OP_TYPE_OD			1


/* GPIO output speeds */

#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPIO_SPEED_HIGH			3


/* GPIO pull-up/pull-down */

#define GPIO_NO_PUPD			0
#define GPIO_PIN_PU				1
#define GPIO_PIN_PD				2


/* GPIO alternate functions */

#define GPIO_PIN_AF0			0
#define GPIO_PIN_AF1			1
#define GPIO_PIN_AF2			2
#define GPIO_PIN_AF3			3
#define GPIO_PIN_AF4			4
#define GPIO_PIN_AF5			5
#define GPIO_PIN_AF6			6
#define GPIO_PIN_AF7			7


/* Clock control */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);


/* Init and De-Init */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);


/* Data read and write */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);


/* IRQ configuration and handling */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_STM32G070XX_GPIO_DRIVER_H_ */
