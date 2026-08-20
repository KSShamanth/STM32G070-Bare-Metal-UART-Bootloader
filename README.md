# STM32G070 Bare-Metal UART Bootloader

A bare-metal UART bootloader for the STM32G070 that receives a firmware `.bin` file from a host PC, erases the application Flash region, programs the firmware, verifies it using CRC-32, and transfers control to the application.

The project also includes custom bare-metal GPIO, UART and Flash drivers implemented using direct register access.

## Quick Start

1. Flash the **Bootloader** project onto the STM32G070 (NUCLEO-G070RB) via ST-Link — this occupies `0x08000000` – `0x08002000`.
2. Build the **Application** project with its linker script set to start at `0x08002000`, and export a `.bin` (not `.elf`).
3. Update `FIRMWARE` and `PORT` in `bootloader_sender.py` to point to your `.bin` file and serial port.
4. Run `python bootloader_sender.py` to push the firmware over UART. On success, the board jumps to the application automatically.

## Features

- Bare-metal STM32G070 development
- Custom GPIO driver
- Custom UART driver
- Custom Flash driver
- UART-based firmware update
- Application Flash erase
- Flash programming using 64-bit programming
- Firmware size validation
- CRC-32 firmware verification
- Application vector table validation
- Bootloader-to-application jump
- ACK/NACK communication
- Bootloader state machine
- Command/state validation
- Python host-side firmware update script
- Positive and negative testing

## System Overview

    Host PC
       |
       | UART
       v
    +-------------------+
    |    Bootloader     |
    |                   |
    | START             |
    | ERASE             |
    | WRITE             |
    | VERIFY            |
    | JUMP              |
    +---------+---------+
              |
              v
    +-------------------+
    | Application Flash |
    |   0x08002000      |
    +---------+---------+
              |
              v
         Application

## Memory Layout

The STM32G070 Flash is divided into a bootloader region and an application region.

    0x08000000  +-------------------------+
                |                         |
                |      Bootloader        |
                |                         |
    0x08002000  +-------------------------+
                |                         |
                |      Application        |
                |                         |
                |                         |
                +-------------------------+

| Region | Address |
|---|---|
| Bootloader Start | `0x08000000` |
| Bootloader Size | `0x2000` |
| Application Start | `0x08002000` |

The application is linked to start at `0x08002000`.

## Firmware Update Flow

The host PC sends the following commands:

    START
      ↓
    ERASE
      ↓
    WRITE
      ↓
    WRITE
      ↓
    ...
      ↓
    VERIFY
      ↓
    JUMP
      ↓
    APPLICATION

## Command Protocol

| Command | Value | Purpose |
|---|---:|---|
| START | `0x01` | Start firmware transfer |
| ERASE | `0x02` | Erase application Flash |
| WRITE | `0x03` | Write firmware data |
| VERIFY | `0x04` | Verify firmware CRC |
| JUMP | `0x05` | Jump to application |
| ACK | `0x06` | Command successful |
| NACK | `0x07` | Command rejected or failed |

## Bootloader State Machine

The bootloader uses a simple state machine to control the firmware update sequence.

    BOOT_IDLE
        |
        | START
        v
    BOOT_STARTED
        |
        | ERASE
        v
    BOOT_ERASED
        |
        | WRITE
        v
    BOOT_WRITING
        |
        | VERIFY
        v
    BOOT_VERIFIED
        |
        | JUMP
        v
    APPLICATION

Only commands valid for the current state are accepted.

For example:

    BOOT_STARTED

    WRITE  -> NACK
    VERIFY -> NACK
    JUMP   -> NACK
    ERASE  -> accepted

## Custom Bare-Metal Drivers

The project includes custom peripheral drivers written using direct register access.

The drivers were developed to understand the MCU peripherals and provide the functionality required by the bootloader.

### GPIO Driver

The GPIO driver covers:

- GPIO clock control
- GPIO initialization and de-initialization
- Input pin and port reading
- Output pin and port writing
- Output pin toggling
- Alternate-function configuration
- EXTI configuration
- NVIC interrupt configuration

Files:

- `stm32g070xx_gpio_driver.h`
- `stm32g070xx_gpio_driver.c`

### UART Driver

The UART driver was not intended to be a complete UART driver implementation.

Only the functionality required to understand and implement this bootloader was developed.

It currently covers:

- UART clock control
- Baud-rate configuration
- Word-length configuration
- Stop-bit configuration
- Parity configuration
- TX/RX configuration
- Blocking UART transmission
- Blocking UART reception

Interrupt-driven UART communication and other advanced UART features are not fully implemented because they were not required for this bootloader.

Files:

- `stm32g070xx_uart_driver.h`
- `stm32g070xx_uart_driver.c`

The bootloader uses:

    UART  : USART2
    Baud  : 115200
    Data  : 8 bits
    Parity: None
    Stop  : 1 bit

### Flash Driver

The Flash driver implements the Flash operations required by the bootloader:

- Flash unlock
- Page erase
- Application-region erase
- 64-bit Flash programming
- Buffer programming
- Flash status/error checking
- Address alignment checking

Files:

- `flash_driver.h`
- `flash_driver.c`

Firmware is programmed using 8-byte Flash programming units.

## CRC Verification

The host calculates the CRC-32 of the complete firmware `.bin` file.

Example:

    Firmware size: 3376 bytes
    Firmware CRC: 0x9584C55B

The size and expected CRC are sent during the `START` command.

After programming, the bootloader calculates the CRC directly from the application Flash.

    CRC32_Calculate((const uint8_t *)APP_START, firmwareSize);

The calculated CRC is compared with the CRC received from the host.

    Host firmware
         |
         | CRC-32
         v
    Expected CRC
         |
         | compare
         v
    Bootloader calculates CRC
         |
         +---- Match ----> ACK
         |
         +---- Mismatch -> NACK

This verifies that the firmware written to Flash matches the firmware sent by the host.

## Application Validation

Before jumping to the application, the bootloader validates the application's vector table.

It checks:

- Initial MSP is within SRAM
- Reset Handler has the Thumb bit set
- Reset Handler is inside the application Flash region

This prevents the bootloader from jumping to an obviously invalid application image.

## Application Jump

After successful verification, the host sends the `JUMP` command.

The bootloader:

1. Reads the application's initial MSP.
2. Reads the application's Reset Handler.
3. Sets the vector table address to `APP_START`.
4. Updates the MSP.
5. Branches to the application's Reset Handler.

The application starts at:

    0x08002000

## Host Firmware Update Script

A Python script is used to send the firmware to the STM32.

The script:

1. Reads the `.bin` file.
2. Calculates the firmware size.
3. Calculates CRC-32.
4. Sends `START`.
5. Sends `ERASE`.
6. Sends firmware in 256-byte packets.
7. Waits for ACK after each packet.
8. Sends `VERIFY`.
9. Waits for verification ACK.
10. Sends `JUMP`.

Example successful update:

    Firmware size: 3376 bytes
    Firmware CRC: 0x9584C55B
    Port: /dev/cu.usbmodem14203

    Sending START...
    START ACK

    Sending ERASE...
    ERASE ACK

    WRITE #1: 256 bytes (0 -> 255)
    WRITE #1 ACK

    ...

    WRITE #14: 48 bytes (3328 -> 3375)
    WRITE #14 ACK

    Sending VERIFY...
    VERIFY ACK

    Sending JUMP...
    JUMP sent

    Firmware update completed successfully.

    Waiting for application...

    APPLICATION:
    This is the application

## Testing

The bootloader was tested using both successful and negative test cases.

### Successful Firmware Update

The complete firmware update sequence was verified:

    START
      ↓
    ERASE
      ↓
    WRITE
      ↓
    VERIFY
      ↓
    JUMP
      ↓
    APPLICATION

The application successfully executed after the bootloader transferred control to it.

### Flash Verification

The application Flash region was checked after erase and after programming.

The application image was programmed starting at:

    0x08002000

Screenshots are included in the `docs` folder.

### Negative Testing

Invalid command sequences were also tested.

Example:

    START
      ↓
    WRITE without ERASE
      ↓
    NACK

This confirms that the bootloader rejects commands that are invalid for the current state.

Other invalid cases were tested, including:

- Invalid firmware size
- Incorrect command order
- Incomplete firmware transfer
- CRC verification failure
- Invalid application image

## Project Structure

    STM32G070-Bare-Metal-UART-Bootloader/
    │
    ├── Bootloader/
    │   ├── Inc/
    │   │   ├── bootloader.h
    │   │   ├── crc.h
    │   │   ├── flash_driver.h
    │   │   ├── stm32g070xx_gpio_driver.h
    │   │   └── stm32g070xx_uart_driver.h
    │   │
    │   └── Src/
    │       ├── bootloader.c
    │       ├── crc.c
    │       ├── flash_driver.c
    │       ├── stm32g070xx_gpio_driver.c
    │       └── stm32g070xx_uart_driver.c
    │
    ├── Application/
    │   └── main.c
    │
    ├── Host/
    │   └── bootloader_sender.py
    │
    ├── docs/
    │   ├── successful_update.png
    │   ├── Flash_mem_after_erase.png
    │   └── Flash_mem_after_successful_update.png
    │
    └── README.md

## Hardware

- STM32G070RB / NUCLEO-G070RB
- USB/UART connection
- Host PC

## Tools and Technologies

- C
- Embedded C
- STM32G070
- ARM Cortex-M0+
- Bare-metal programming
- Direct register access
- UART
- GPIO
- Flash memory
- CRC-32
- Python
- STM32CubeIDE

## Limitations

This project focuses on understanding and implementing the functionality required for a basic firmware update system.

Current limitations include:

- Blocking UART communication
- UART interrupt-driven communication is not fully implemented
- No firmware encryption
- No digital signature/authentication
- No rollback mechanism
- No power-loss recovery mechanism
- No firmware version management
- Basic ACK/NACK communication protocol
- No packet sequence numbers
- No retransmission mechanism
- No receive timeout — a disconnected host during a transfer will block the bootloader indefinitely

These features were outside the scope of the current implementation.

## Future Improvements

Possible improvements include:

- UART interrupt-driven communication
- Packet sequence numbers
- Packet-level CRC
- Timeout and retry mechanism
- Firmware authentication/signing
- Firmware version checking
- Rollback support
- Watchdog-based recovery
- Power-loss recovery
- Non-blocking communication
- Peripheral/interrupt state cleanup before jumping to the application

## What This Project Demonstrates

This project combines low-level MCU programming with a practical firmware-update workflow.

It demonstrates:

- Register-level peripheral programming
- Embedded C
- GPIO and UART driver development
- Flash memory management
- Flash programming and erase operations
- UART communication protocol design
- State-machine based firmware control
- CRC-based firmware integrity verification
- Application vector table validation
- Bootloader-to-application handoff
- Host-side firmware update scripting
- Positive and negative testing

## Summary

The project implements a complete basic UART firmware update flow on the STM32G070:

    Custom Drivers
          |
          v
    UART Communication
          |
          v
    Bootloader State Machine
          |
          +---- Flash Erase
          |
          +---- Firmware Programming
          |
          +---- CRC Verification
          |
          +---- Application Validation
          |
          v
    Application Jump

The result is a working bare-metal bootloader capable of receiving, programming, verifying and executing a firmware application over UART.

## License

No license specified — personal/portfolio project.