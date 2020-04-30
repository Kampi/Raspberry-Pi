#!/usr/bin/python

import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

TRIGGER = 24
ECHO = 23

# Schallgeschwindigkeit in m/s
Schallgeschwindigkeit = 331

print "Ultraschall Entfernungsmessung"

while True:

	GPIO.setup(TRIGGER,GPIO.OUT)
	GPIO.setup(ECHO,GPIO.IN)

	# Messung ausloesen
	GPIO.output(TRIGGER, False)
	time.sleep(0.5)
	GPIO.output(TRIGGER, True)
	time.sleep(0.00001)
	GPIO.output(TRIGGER, False)

	while GPIO.input(ECHO) == 0:
		pass

	# Startzeit festhalten
	Start = time.time()

	while GPIO.input(ECHO) == 1:
		pass
	
	# Stopzeit festhalten
	Stop = time.time()

	# Zeitdifferenz berechnen
	Zeit = Stop - Start

	# Entfernung in Meter berechnen
	Entfernung = Zeit * (Schallgeschwindigkeit/2)
	Entfernung = Entfernung * 100
	Entfernung = round(Entfernung, 2)

        print "Abstand: ", Entfernung, " cm"
        time.sleep(1)

        GPIO.cleanup()