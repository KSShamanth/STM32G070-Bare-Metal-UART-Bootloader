
#include "bootloader.h"
#include "crc.h"
#include "flash_driver.h"
#include "stm32g070xx.h"


/* Private helper functions */

static void Bootloader_SendAck(UART_Handle_t *pUART);

static void Bootloader_SendNack(UART_Handle_t *pUART);

static bool Bootloader_HandleWrite(
        UART_Handle_t *pUART,
        uint32_t firmwareSize,
        uint32_t *currentAddress,
        uint32_t *bytesReceived,
        uint8_t *rxBuffer);

static bool Bootloader_HandleVerify(
        UART_Handle_t *pUART,
        uint32_t firmwareSize,
        uint32_t firmwareCRC,
        uint32_t bytesReceived);


/* ACK */

static void Bootloader_SendAck(UART_Handle_t *pUART)
{
    uint8_t response = BL_ACK;

    UART_SendData(
            pUART,
            &response,
            1);
}


/* NACK */

static void Bootloader_SendNack(UART_Handle_t *pUART)
{
    uint8_t response = BL_NACK;

    UART_SendData(
            pUART,
            &response,
            1);
}


/* Application validation */

bool Bootloader_IsApplicationValid(void)
{
    uint32_t msp = APP_STACK_PTR;
    uint32_t resetHandler = APP_RESET_HANDLER;


    /* Check MSP */

    if ((msp < 0x20000000U) ||
        (msp > 0x20009000U))
    {
        return false;
    }


    /* Check Thumb bit */

    if ((resetHandler & 0x00000001U) == 0U)
    {
        return false;
    }


    /* Remove Thumb bit */

    resetHandler &= 0xFFFFFFFEU;


    /* Check Reset Handler address */

    if ((resetHandler < APP_START) ||
        (resetHandler >= (APP_START + APP_MAX_SIZE)))
    {
        return false;
    }


    return true;
}


/* Jump to application */

__attribute__((noreturn))
void Bootloader_JumpToApplication(void)
{
    uint32_t appMSP = APP_STACK_PTR;
    uint32_t appResetHandler = APP_RESET_HANDLER;


    /* Set application vector table */

    SCB->VTOR = APP_START;


    /* Set MSP and jump to application */

    __asm volatile
    (
        "MSR MSP, %0\n"
        "DSB\n"
        "ISB\n"
        "BX %1\n"
        :
        : "r"(appMSP),
          "r"(appResetHandler)
        : "memory"
    );


    /* Should never reach here */

    while (1)
    {
    }
}


/* Handle WRITE */

static bool Bootloader_HandleWrite(
        UART_Handle_t *pUART,
        uint32_t firmwareSize,
        uint32_t *currentAddress,
        uint32_t *bytesReceived,
        uint8_t *rxBuffer)
{
    uint32_t writeLength;
    uint32_t remaining;


    /* Receive packet length */

    UART_ReceiveData(
            pUART,
            (uint8_t *)&writeLength,
            4);


    remaining = firmwareSize - *bytesReceived;


    /* Validate packet length */

    if ((writeLength == 0U) ||
        (writeLength > BOOTLOADER_RX_BUFFER_SIZE) ||
        (writeLength > remaining))
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Check packet alignment */

    if ((writeLength < remaining) &&
        ((writeLength % 8U) != 0U))
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Receive firmware data */

    UART_ReceiveData(
            pUART,
            rxBuffer,
            writeLength);


    /* Program Flash */

    if (!FLASH_ProgramBuffer(
            *currentAddress,
            rxBuffer,
            writeLength))
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Update transfer information */

    *currentAddress += writeLength;

    *bytesReceived += writeLength;


    /* Send ACK */

    Bootloader_SendAck(pUART);

    return true;
}


/* Handle VERIFY */

static bool Bootloader_HandleVerify(
        UART_Handle_t *pUART,
        uint32_t firmwareSize,
        uint32_t firmwareCRC,
        uint32_t bytesReceived)
{
    uint32_t calculatedCRC;


    /* Check firmware size */

    if (bytesReceived != firmwareSize)
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Check application */

    if (!Bootloader_IsApplicationValid())
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Calculate CRC */

    calculatedCRC =
        CRC32_Calculate(
            (const uint8_t *)APP_START,
            firmwareSize);


    /* Compare CRC */

    if (calculatedCRC != firmwareCRC)
    {
        Bootloader_SendNack(pUART);

        return false;
    }


    /* Verification successful */

    Bootloader_SendAck(pUART);

    return true;
}


/* Bootloader state machine */

void Bootloader_Run(UART_Handle_t *pUART)
{
    uint8_t command;
    uint8_t lengthBytes[4];

    uint32_t firmwareCRC = 0U;
    uint32_t firmwareSize = 0U;

    uint32_t currentAddress = APP_START;
    uint32_t bytesReceived = 0U;

    uint8_t rxBuffer[BOOTLOADER_RX_BUFFER_SIZE];

    BootState_t currentState = BOOT_IDLE;


    while (1)
    {
        /* Receive command */

        UART_ReceiveData(
                pUART,
                &command,
                1);


        switch (currentState)
        {
            /* BOOT IDLE */

            case BOOT_IDLE:

                if (command == BL_CMD_START)
                {
                    /* Receive firmware size */

                    UART_ReceiveData(
                            pUART,
                            lengthBytes,
                            sizeof(lengthBytes));


                    /* Convert size */

                    firmwareSize = 0U;

                    for (uint32_t i = 0U;
                         i < sizeof(lengthBytes);
                         i++)
                    {
                        firmwareSize |=
                            ((uint32_t)lengthBytes[i])
                            << (8U * i);
                    }


                    /* Receive CRC */

                    UART_ReceiveData(
                            pUART,
                            (uint8_t *)&firmwareCRC,
                            sizeof(firmwareCRC));


                    /* Validate firmware size */

                    if ((firmwareSize > 0U) &&
                        (firmwareSize <= APP_MAX_SIZE))
                    {
                        currentAddress = APP_START;

                        bytesReceived = 0U;

                        currentState = BOOT_STARTED;

                        Bootloader_SendAck(pUART);
                    }
                    else
                    {
                        Bootloader_SendNack(pUART);
                    }
                }
                else
                {
                    Bootloader_SendNack(pUART);
                }

                break;


            /* BOOT STARTED */

            case BOOT_STARTED:

                if (command == BL_CMD_ERASE)
                {
                    FLASH_Unlock();

                    if (FLASH_EraseApplication())
                    {
                        currentState = BOOT_ERASED;

                        Bootloader_SendAck(pUART);
                    }
                    else
                    {
                        Bootloader_SendNack(pUART);
                    }
                }
                else
                {
                    Bootloader_SendNack(pUART);
                }

                break;


            /* BOOT ERASED */

            case BOOT_ERASED:

                if (command == BL_CMD_WRITE)
                {
                    if (Bootloader_HandleWrite(
                            pUART,
                            firmwareSize,
                            &currentAddress,
                            &bytesReceived,
                            rxBuffer))
                    {
                        currentState = BOOT_WRITING;
                    }
                }
                else
                {
                    Bootloader_SendNack(pUART);
                }

                break;


            /* BOOT WRITING */

            case BOOT_WRITING:

                if (command == BL_CMD_WRITE)
                {
                    Bootloader_HandleWrite(
                            pUART,
                            firmwareSize,
                            &currentAddress,
                            &bytesReceived,
                            rxBuffer);
                }
                else if (command == BL_CMD_VERIFY)
                {
                    if (Bootloader_HandleVerify(
                            pUART,
                            firmwareSize,
                            firmwareCRC,
                            bytesReceived))
                    {
                        currentState = BOOT_VERIFIED;
                    }
                }
                else
                {
                    Bootloader_SendNack(pUART);
                }

                break;


            /* BOOT VERIFIED */

            case BOOT_VERIFIED:

                if (command == BL_CMD_JUMP)
                {
                    Bootloader_JumpToApplication();
                }
                else
                {
                    Bootloader_SendNack(pUART);
                }

                break;


            /* Unknown state */

            default:

                Bootloader_SendNack(pUART);

                currentState = BOOT_IDLE;

                break;
        }
    }
}
