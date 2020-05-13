import serial

UART = serial.Serial("/dev/serial0", 9600)

UART.write(chr(0xFE))
UART.write(chr(0x9C))

UART.write("Test")