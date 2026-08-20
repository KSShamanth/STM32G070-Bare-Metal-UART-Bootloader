/*
 * flash_driver.c
 *
 *  Created on: 14-Aug-2026
 *      Author: apple
 */

#include <stdint.h>
#include <stdbool.h>
#include "flash_driver.h"
#include "stm32g070xx.h"
#include "bootloader.h"

void FLASH_Unlock(void){
	if ((FLASH->CR >> 31) & 1U){ //if register is locked
		FLASH->KEYR = 0x45670123U;
		FLASH->KEYR = 0xCDEF89ABU;	// unlock them
	}
}

bool FLASH_ErasePage(uint32_t pageNumber)
{
    /* Wait until Flash is not busy */
    while ((FLASH->SR >> 16) & 1U) {
    }

    /* Clear previous Flash status/error flags */
    FLASH->SR = FLASH_SR_CLEAR_ERRORS;

    /* Enable page erase */
    FLASH->CR |= (1U << 1);

    /* Clear existing PNB[9:0] */
    FLASH->CR &= ~(0x3FFU << 3);

    /* Set page number */
    FLASH->CR |= ((pageNumber & 0x3FFU) << 3);

    /* Start page erase */
    FLASH->CR |= (1U << 16);

    /* Wait until erase is complete */
    while ((FLASH->SR >> 16) & 1U) {
    }

    /* Check for Flash errors */
    if (FLASH->SR & (
            FLASH_SR_OPERR  |
            FLASH_SR_PROGERR |
            FLASH_SR_WRPERR |
            FLASH_SR_PGAERR |
            FLASH_SR_SIZERR |
            FLASH_SR_PGSERR |
            FLASH_SR_MISSERR |
            FLASH_SR_FASTERR))
    {
        FLASH->CR &= ~(1U << 1);     // PER = 0
        return false;
    }

    /* Disable page erase */
    FLASH->CR &= ~(1U << 1);

    return true;
}

bool FLASH_EraseApplication(void){

	uint32_t firstPage = (APP_START - BOOTLOADER_START) / FLASH_PAGE_SIZE;

	uint32_t noOfPages = APP_MAX_SIZE / FLASH_PAGE_SIZE;

	for (uint32_t page = firstPage;page < firstPage + noOfPages;page++){
		if (!(FLASH_ErasePage(page))){
			return false;
		}
	}
	return true;

}


bool FLASH_ProgramDoubleWord(uint32_t address, uint64_t data)
{
    /* Wait until no Flash operation is in progress */
    while ((FLASH->SR >> 16) & 1U) {
        // wait
    }

    /* Flash programming requires 8-byte aligned address */
    if (address % 8) {
        return false;
    }

    /* Clear previous Flash error flags */
    FLASH->SR = FLASH_SR_CLEAR_ERRORS;

    /* Enable Flash programming */
    FLASH->CR |= (1U << 0);

    /* Write the lower 32 bits of the double word */
    *(volatile uint32_t *)address = data & 0xFFFFFFFFU;

    /* Write the upper 32 bits of the double word */
    *(volatile uint32_t *)(address + 4) = data >> 32;

    /* Wait until the programming operation is complete */
    while ((FLASH->SR >> 16) & 1U) {
        // wait
    }

    /* Check whether any Flash programming error occurred */
    if (FLASH->SR & (
            FLASH_SR_OPERR  |
            FLASH_SR_PROGERR |
            FLASH_SR_WRPERR |
            FLASH_SR_PGAERR |
            FLASH_SR_SIZERR |
            FLASH_SR_PGSERR |
            FLASH_SR_MISSERR |
            FLASH_SR_FASTERR))
    {
        /* Disable Flash programming */
        FLASH->CR &= ~(1U << 0);

        return false;
    }
    /* Disable Flash programming */
    FLASH->CR &= ~(1U << 0);

    return true;

}

bool FLASH_ProgramBuffer(uint32_t address,const uint8_t *data,uint32_t length)
{
    uint32_t size;
    uint32_t remainder;
    uint32_t programmedLength;


    /* Check address alignment */

    if (address & 0x7U)
    {
        return false;
    }


    /* Check length */

    if (length == 0U)
    {
        return false;
    }


    /* Check application address */

    if (address < APP_START)
    {
        return false;
    }


    /*
     * Calculate the actual number of bytes that will be
     * programmed. The final partial double word is padded
     * with zeroes.
     */

    programmedLength = (length + 7U) & ~7U;


    /* Check application boundary */

    if (programmedLength > (APP_END - address))
    {
        return false;
    }


    size = length / 8U;


    /* Program complete double words */

    while (size > 0U)
    {
        uint64_t value = 0U;

        for (uint32_t i = 0U; i < 8U; i++)
        {
            value |=
                ((uint64_t)*data) << (8U * i);

            data++;
        }


        if (!FLASH_ProgramDoubleWord(
                address,
                value))
        {
            return false;
        }


        address += 8U;

        size--;
    }


    /* Program remaining bytes with zero padding */

    remainder = length % 8U;

    if (remainder != 0U)
    {
        uint64_t value = 0U;

        for (uint32_t i = 0U; i < remainder; i++)
        {
            value |=
                ((uint64_t)*data) << (8U * i);

            data++;
        }


        if (!FLASH_ProgramDoubleWord(
                address,
                value))
        {
            return false;
        }
    }


    return true;
}








