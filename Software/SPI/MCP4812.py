import time
import spidev
import RPi.GPIO as GPIO

# Chip-Select for the DAC
CS = 25

GPIO.setwarnings(False)

# Configure the CS pin as output
GPIO.setmode(GPIO.BCM)
GPIO.setup(CS, GPIO.OUT)

# Set the CS pin to high state
GPIO.output(CS, GPIO.HIGH)

# Open SPI device
SPI = spidev.SpiDev()
SPI.open(0, 0)

# Disable the internal CS
SPI.no_cs = True

while True:
	for I in range(0, 256, 10):
		# Clear the CS pin
		GPIO.output(CS, GPIO.LOW)

		# Transmit data
		SPI.writebytes([144, I])

		# Set the CS pin
		GPIO.output(CS, GPIO.HIGH)

		time.sleep(1)
