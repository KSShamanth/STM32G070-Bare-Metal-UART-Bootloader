
#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32g070xx_uart_driver.h"

typedef enum
{
    BOOT_IDLE,
    BOOT_STARTED,
    BOOT_ERASED,
    BOOT_WRITING,
    BOOT_VERIFIED
} BootState_t;

#define BL_CMD_START    				0x01
#define BL_CMD_ERASE    				0x02
#define BL_CMD_WRITE    				0x03
#define BL_CMD_VERIFY   				0x04
#define BL_CMD_JUMP     				0x05

#define BL_ACK           				0x06
#define BL_NACK          				0x07

#define BOOTLOADER_RX_BUFFER_SIZE    	256U


#define BOOTLOADER_START 				0x08000000U

#define BOOTLOADER_SIZE 				0x2000U

#define APP_START 						((BOOTLOADER_START) + (BOOTLOADER_SIZE))

#define APP_MAX_SIZE 					(0x20000 - (BOOTLOADER_SIZE))

#define APP_END             			(APP_START + APP_MAX_SIZE)

#define APP_STACK_PTR 					(*(volatile uint32_t*) (APP_START))

#define APP_RESET_HANDLER 				(*(volatile uint32_t*) (APP_START + 4U))



bool Bootloader_IsApplicationValid(void);

 __attribute__((noreturn)) void Bootloader_JumpToApplication(void);

 void Bootloader_Run(UART_Handle_t *pUART);

#endif /* BOOTLOADER_H_ */
