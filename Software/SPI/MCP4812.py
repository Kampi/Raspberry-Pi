import spidev
import time
import RPi.GPIO as GPIO

# Chipselect Pin
CS = 25

# Warnungen deaktivieren
GPIO.setwarnings(False)

# GPIO 25 als CS konfigurieren
GPIO.setmode(GPIO.BCM)
GPIO.setup(CS, GPIO.OUT)

# Pin auf High setzen
GPIO.output(CS, GPIO.HIGH)

# SPI Device oeffnen
SPI = spidev.SpiDev()
SPI.open(0,0)

while True:
	for I in range(0,256,10):
	
		# Chipselect auf Low setzen
		GPIO.output(CS, GPIO.LOW)

		# Daten senden
		SPI.writebytes([144, I])

		# Chipselect auf High setzen
		GPIO.output(CS, GPIO.HIGH)

		time.sleep(1)
