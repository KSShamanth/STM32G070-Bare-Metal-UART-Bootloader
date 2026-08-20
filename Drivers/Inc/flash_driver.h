
#ifndef INC_FLASH_DRIVER_H_
#define INC_FLASH_DRIVER_H_

#include "stm32g070xx.h"
#include <stdbool.h>

#define FLASH_SR_OPTVERR   (1U << 15)
#define FLASH_SR_FASTERR   (1U << 9)
#define FLASH_SR_MISSERR   (1U << 8)
#define FLASH_SR_PGSERR    (1U << 7)
#define FLASH_SR_SIZERR    (1U << 6)
#define FLASH_SR_PGAERR    (1U << 5)
#define FLASH_SR_WRPERR    (1U << 4)
#define FLASH_SR_PROGERR   (1U << 3)
#define FLASH_SR_OPERR     (1U << 1)
#define FLASH_SR_EOP       (1U << 0)

#define FLASH_PAGE_SIZE	   0x800U

#define FLASH_SR_CLEAR_ERRORS \
    (FLASH_SR_OPTVERR  | FLASH_SR_FASTERR | FLASH_SR_MISSERR | \
     FLASH_SR_PGSERR   | FLASH_SR_SIZERR  | FLASH_SR_PGAERR  | \
     FLASH_SR_WRPERR   | FLASH_SR_PROGERR | FLASH_SR_OPERR)


void FLASH_Unlock(void);

bool FLASH_ErasePage(uint32_t pageNumber);

bool FLASH_EraseApplication(void);

bool FLASH_ProgramDoubleWord(uint32_t address, uint64_t data);

bool FLASH_ProgramBuffer(uint32_t address,const uint8_t *data,uint32_t length);

#endif /* INC_FLASH_DRIVER_H_ */
