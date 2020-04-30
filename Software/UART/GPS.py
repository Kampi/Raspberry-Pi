#!/usr/bin/python
# -*- coding: utf-8 -*-

import serial
import time

# Dictionary fuer die empfangene Daten
Protokolle = {}

# Variablen fuer die Uhrzeit
Uhrzeit = ""
Datum = ""
Stunde = ""
Minute = ""
Sekunde = ""
Tag = ""
Monat = ""
Jahr = ""

# Koordinaten
Koordinaten_Laenge = 0.0
Koordinaten_Breite = 0.0
Breitengrad = 0.0
Laengengrad = 0.0
Kurs = 0.0
Winkelminuten_Breite = 0.0
Winkelminuten_Laenge = 0.0
Winkelsekunden_Breite = 0.0
Winkelsekunden_Laenge = 0.0
Hoehe = 0.0

# Geschwindigkeit
Geschwindigkeit_Knoten = 0.0
Geschwindigkeit_Kilometer = 0.0

# Sonstiges
Status_GPRMC = ""
Quali_GPGGA = ""
Richtung_1_GPRMC = ""
Richtung_2_GPRMC = ""
Anzahl_Sat = ""

# UART oeffnen
UART = serial.Serial("/dev/ttyUSB0", 4800)
UART.open()

# Startmeldung
print ""
print "+---------------------------------------------------------------------+"
print "| Dieses Programm empfaengt Daten einer Holux GPS-Maus.               |"
print "| Der empfangene Datensatz wird zerlegt und in der Konsole angezeigt. |"
print "+---------------------------------------------------------------------+"
print ""

while True:

	Protokolle = {}

	for Counter in range(3):
	
		Daten = []
	
		# Zeile einlesen
		Zeile = UART.readline()
		Zeile = Zeile.strip()

		# Zeile trennen
		Daten = Zeile.split(",")

		# Daten dem Dictionary hinzufuegen
		Schluessel = Daten[0]
		del Daten[0]
		Protokolle.update({Schluessel:Daten})

	for Key in Protokolle.iterkeys():

		# Daten extrahieren
		if(Key == "$GPRMC"):

			# Datum formatieren
			Uhrzeit = Protokolle[Key][0]
			Stunde = Uhrzeit[0:2]
			Minute = Uhrzeit[2:4]
			Sekunde = Uhrzeit[4:6]
			Datum = Protokolle[Key][8]
			Tag = Datum[0:2]
			Monat = Datum[2:4]
			Jahr = Datum[4:6]

			# Koordinaten
			Breitengrad = int(float(Protokolle[Key][2]) / 100.0)
			Laengengrad = int(float(Protokolle[Key][4]) / 100.0)
			Winkelminuten_Breite = int(float(Protokolle[Key][2][2:]))
			Winkelminuten_Laenge = int(float(Protokolle[Key][4][3:]))
			Winkelsekunden_Breite = float(Protokolle[Key][2][4:]) * 100.0
			Winkelsekunden_Laenge = float(Protokolle[Key][4][5:]) * 100.0

			# Koordinaten umrechnen
			Koordinaten_Breite = round((((float(Winkelsekunden_Breite) / 60.0) + float(Winkelminuten_Breite))/60.0) + float(Breitengrad), 6)
			Koordinaten_Laenge = round((((float(Winkelsekunden_Laenge) / 60.0) + float(Winkelminuten_Laenge))/60.0) + float(Laengengrad), 6)

			# Geschwindigkeit auslesen
			Geschwindigkeit_Knoten = float(Protokolle[Key][2])
			Geschwindigkeit_Kilometer = Geschwindigkeit_Knoten * 1.852

			# Sonstiges
			Status_GPRMC = Protokolle[Key][1]
			Richtung_1_GPRMC = Protokolle[Key][3]
			Richtung_2_GPRMC = Protokolle[Key][5]

		elif(Key == "$GPGGA"):

			# Koordinaten
			Hoehe = float(Protokolle[Key][8])

			# Sonstiges
			Quali_GPGGA = Protokolle[Key][5]
			Anzahl_Sat = Protokolle[Key][6]

		else:
			pass

	# Ausgabe
	print "Datum: " + Tag + "/" + Monat + "/" + Jahr
	print "Uhrzeit: " + Stunde + ":" + Minute + ":" + Sekunde
	print "Status: " + Status_GPRMC
	print "Qualitaet: " + Quali_GPGGA
	print "Anzahl Satelliten: " + Anzahl_Sat
	print "Koordinaten Breite: " + str(Koordinaten_Breite)
	print "Koordinaten Laenge: " + str(Koordinaten_Laenge)
	print "Breitengrad: " + Richtung_1_GPRMC + " " + str(Breitengrad) + "°" + str(Winkelminuten_Breite) + "'" + str(Winkelsekunden_Breite) + "''"
	print "Laengengrad: " + Richtung_2_GPRMC + " " + str(Laengengrad) + "°" + str(Winkelminuten_Laenge) + "'" + str(Winkelsekunden_Laenge) + "''"
	print "Hoehe vom MSL: " + str(Hoehe)
	print "Geschwindigkeit: " + str(Geschwindigkeit_Knoten) + " Knoten" + " - " + str(Geschwindigkeit_Kilometer) + " km/h"
	print ""	
	
	time.sleep(4)
	UART.flushInput()