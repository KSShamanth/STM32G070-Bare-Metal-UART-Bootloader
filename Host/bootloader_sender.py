import serial
import struct
import time
import zlib

PORT = "/dev/cu.usbmodem14203"
BAUDRATE = 115200

FIRMWARE = "/Users/apple/Desktop/Embedded C/My_workspace/target/009uart_bootloader_app/Debug/009uart_bootloader_app.bin"

START = 0x01
ERASE = 0x02
WRITE = 0x03
VERIFY = 0x04
JUMP = 0x05

ACK = 0x06
NACK = 0x07

CHUNK_SIZE = 256


def wait_for_ack(ser):

    response = ser.read(1)

    if len(response) == 0:
        print("ERROR: Timeout waiting for response")
        return False

    if response[0] == ACK:
        return True

    if response[0] == NACK:
        print("ERROR: Bootloader returned NACK")
        return False

    print(f"ERROR: Unexpected response: 0x{response[0]:02X}")
    return False


def send_command(ser, command):

    ser.write(bytes([command]))
    ser.flush()


def main():

    # Read firmware

    with open(FIRMWARE, "rb") as f:
        firmware = f.read()

    firmware_size = len(firmware)

    # Calculate CRC-32/ISO-HDLC

    firmware_crc = zlib.crc32(firmware) & 0xFFFFFFFF

    print(f"Firmware: {FIRMWARE}")
    print(f"Firmware size: {firmware_size} bytes")
    print(f"Firmware CRC: 0x{firmware_crc:08X}")
    print(f"Port: {PORT}")
    print()


    # Open serial port

    ser = serial.Serial(
        PORT,
        BAUDRATE,
        timeout=3,
        write_timeout=3
    )

    time.sleep(0.2)


    # -------------------------
    # START
    # -------------------------

    print("Sending START...")

    packet = bytes([START])
    packet += struct.pack("<I", firmware_size)
    packet += struct.pack("<I", firmware_crc)

    ser.write(packet)
    ser.flush()

    if not wait_for_ack(ser):
        print("START failed")
        ser.close()
        return

    print("START ACK")


    # -------------------------
    # ERASE
    # -------------------------

    print("Sending ERASE...")

    send_command(ser, ERASE)

    if not wait_for_ack(ser):
        print("ERASE failed")
        ser.close()
        return

    print("ERASE ACK")


    # -------------------------
    # WRITE
    # -------------------------

    offset = 0
    packet_number = 1

    while offset < firmware_size:

        remaining = firmware_size - offset
        chunk_size = min(CHUNK_SIZE, remaining)

        chunk = firmware[offset:offset + chunk_size]

        print(
            f"WRITE #{packet_number}: "
            f"{chunk_size} bytes "
            f"({offset} -> {offset + chunk_size - 1})"
        )

        packet = bytes([WRITE])
        packet += struct.pack("<I", chunk_size)
        packet += chunk

        ser.write(packet)
        ser.flush()

        if not wait_for_ack(ser):
            print(f"WRITE #{packet_number} failed")
            ser.close()
            return

        print(f"WRITE #{packet_number} ACK")

        offset += chunk_size
        packet_number += 1


    # -------------------------
    # VERIFY
    # -------------------------

    print("Sending VERIFY...")

    send_command(ser, VERIFY)

    if not wait_for_ack(ser):
        print("VERIFY failed")
        ser.close()
        return

    print("VERIFY ACK")


    # -------------------------
    # JUMP
    # -------------------------

    print("Sending JUMP...")

    send_command(ser, JUMP)

    print("JUMP sent")
    print()
    print("Firmware update completed successfully.")
    print()
    print("Waiting for application...")
    print()


    # -------------------------
    # APPLICATION OUTPUT
    # -------------------------

    ser.timeout = 5

    application_output = ser.read_until(b"application")

    if application_output:

        print("APPLICATION:")
        print(application_output.decode(errors="replace"))

    else:

        print("ERROR: No application output received.")


    ser.close()


if __name__ == "__main__":
    main()
