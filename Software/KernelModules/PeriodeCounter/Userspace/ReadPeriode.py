#!/usr/bin/python3

import os
import time

if(__name__ == "__main__"):
	with open("/proc/PeriodeCounter/Resolution", "r") as File:
		Resolution = File.readline().strip()
		print("[INFO] Resultion set to '{}'".format(Resolution))
	
	try:
		while(True):

			with open("/dev/PeriodeCounter", "r") as File:
				print("[INFO] Periode: {} {}".format(File.readline().strip(), Resolution))
			time.sleep(1)

	except KeyboardInterrupt:
		print("[INFO] Exit programm...")
		quit()
