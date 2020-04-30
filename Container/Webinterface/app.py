import sys
import RPi.GPIO as GPIO

from Webservice import Webservice
from ObserveableData import ObserveableData

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)

Running = True
Data = {}

def applicationStatusChanged(self, Value):
	global Running
	
	if(Value == "Off"):
		Running = False

def gpioOnChanged(self, Value):
	GPIO.output(int(Value), GPIO.HIGH)

def gpioOffChanged(self, Value):
	GPIO.output(int(Value), GPIO.LOW)

def main():
	global Webservice

	while(Running):
		pass

	GPIO.cleanup()
	print("[INFO] Close application...")
	sys.exit()

if(__name__ == "__main__"):

	Data.update({ "applicationStatus" : ObserveableData("On") })
	Data["applicationStatus"].set_Callback(applicationStatusChanged)

	Data.update({ "gpio_on" : ObserveableData("") })
	Data["gpio_on"].set_Callback(gpioOnChanged)

	Data.update({ "gpio_off" : ObserveableData("") })
	Data["gpio_off"].set_Callback(gpioOffChanged)

	# Start the webservice
	Webservice = Webservice(Data) 
	Webservice.Run()     
	
	# Register some data keys
	Webservice.GetDataFromWeb("applicationStatus")
	Webservice.GetDataFromWeb("gpio_on")
	Webservice.GetDataFromWeb("gpio_off")

	main()