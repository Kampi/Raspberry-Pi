import serial

"""
Funktionen:
Close()											Beendet das Modul
PrintTestpage()									Druckt eine Testseite aus
Standby(Modus)									Versetzt den Drucker in einen anderen Modus
	Offline											Standbymodus
	Online											Normaler Betrieb
Sleep(Zeit)										Versetzt den Drucker in den Sleep-Modus
	Zeit											Zeit bis in den Sleep-Modus geschaltet wird
Wake()											Weckt den Drucker aus dem Sleep-Modus auf
Status()										Gibt den Papierstatus zurueck
	0 => Papier vorhanden
	1 => Kein Papier vorhanden
ConfigHeat(Dots, Time, Intervall)				Konfiguriert die Heizpunkte des Druckers
	Dots
	Time
	Intervall
DefaultHeat()									Stellt die Standardeinstellungen der Heizpunkte wieder her	
Println(Text)									Schreibt eine Zeile Text
	Text											Zu schreibender Text
Underline(Dicke)								Stellt verschiedene Linienstaerken zum Unterstreichen des Textes ein
	None											Keine
	Middle											Mittel
	Big												Dick
Linefeed(Anzahl)								Druckt eine bestimmte Anzahl Linefeeds
	Anzahl											Anzahl Linefeeds (0-255)
SetLinespace(Punkte)							Definiert den Abstand zwischen zwei Zeilen
	Punkte											Anzahl der Punkte zwischen zwei Zeilen (0-255)
SetLineDefault()								Setzt den Zeilenabstand auf den Defaultwert zurueck (32 Punkte)
Printmode(Mode, Enable)							Stellt verschiedene Druckmodi ein
	Enable											Aktiviert oder Deaktiviert den Modus
	Reverse
	Updown
	Bold
	DoubleHeight
	DoubleWidth
	Deleteline
Tab()											Druckt ein Tab
PrintBarcode(Daten, Typ)						Druckt einen Barcode
	Daten											Liste mit den Daten fuer den Barcode
	Typ												Legt den Barcodetyp fest
BarcodeHeight(Hoehe)							Stellt die Hoehe des Barcodes ein
	Hoehe											Die Hoehe des Barcodes (0-255)
BarcodeWidth(Breite)							Legt die Breite des Barcodes fest
	Small											
	Big
BarcodePosition(Position)						Legt die Position der Zeichen am Barcode fest
	Oben
	Unten
	Beides
"""
class ThermalPrinter:

	# Konstruktor
	def __init__(self, device, baudrate):
	
		self.device = device
		self.baudrate = baudrate
		self.Enable = False

		# UART initalisieren
		self.UART = serial.Serial(self.device, self.baudrate)

		# Schnittstelle oeffnen
		if(not(self.UART.isOpen())):
			self.UART.open()

			self.Enable = True

		self.UART.write(chr(27))
		self.UART.write(chr(64))

		return

	# Beendet die Schnittstelle
	def Close(self):

		# Pruefen ob Schnittstelle geoeffnet wurde
		if(self.Enable):

			self.UART.close()
			self.Enable = False
	
		return
	
	# ------------------------------------------------------------------------
	#				Drucker
	# ------------------------------------------------------------------------

	# Testseite drucken
	def PrintTestpage(self):

		self.UART.write(chr(18))
		self.UART.write(chr(84))

		return

	# Standby auswaehlen
	# Auswahl
	# - Offline
	# - Online
	def Standby(self, Modus):

		if(Modus == "Offline"): 
			Value = 0
		elif(Modus == "Online"):
			Value = 1

		self.UART.write(chr(27))
		self.UART.write(chr(61))
		self.UART.write(chr(Value))

		return

	# Drucker in Sleepmodus setzen
	# WICHTIG: Der Drucker muss erst mittels "Wake()" geweckt werden, wenn er wieder benutzt werden soll
	# Auswahl
	# - Zeit von 0-255	
	def Sleep(self, Zeit):

		if(Zeit > 255):
			print "Der Wert fuer die Zeit ist zu hoch!"
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(56))
		self.UART.write(chr(Zeit))

		return

	# Drucker aufwecken
	def Wake(self):

		UART.write(chr(255))
		time.sleep(0.1)

		return

	# Papierstatus abfragen
	def Status(self):

		Status = 1
	
		self.UART.write(chr(27))
		self.UART.write(chr(118))
		self.UART.write(chr(0))

		# Zeichen einlesen
		Read = self.UART.read(1)

		if(Read == chr(32)):
			Status = 0
		elif(Read == chr(36)):
			Status = 1

		return Status
	
	# Heizzeit konfigurieren
	# Auswahl
	# - Anzahl der Heizpunkte von 0-255
	# - Heizzeit von 3-255
	# - Heizintervall 0-255
	def ConfigHeat(self, Dots, Time, Intervall):

		if(Dots > 255):
			print "Anzahl der Heizpunkte zu hoch!"
			return -1
		if((Time < 3) or (Time > 255)):
			print "Ungueltige Angabe fuer die Heizzeit!"
			return -1
		if(Intervall > 255):
			print "Heizintervall zu hoch!"
			return -1	

		self.UART.write(chr(27))
		self.UART.write(chr(55))
		self.UART.write(chr(Dots))
		self.UART.write(chr(Time))
		self.UART.write(chr(Intervall))

		return

	# Default Einstellungen fuer die Heizung
	def DefaultHeat(self):

		self.UART.write(chr(27))
		self.UART.write(chr(55))
		self.UART.write(chr(7))
		self.UART.write(chr(80))
		self.UART.write(chr(2))
	
		return
	
	# ------------------------------------------------------------------------
	#				Character
	# ------------------------------------------------------------------------
	
	# Drucken einer Zeile
	def Println(self, Text):

		self.UART.write(Text)
		self.UART.write(chr(10))
		self.UART.write(chr(13))

		return

	# Linienstaerke einstellen:
	# Auswahl
	# - None
	# - Middel	
	# - Big
	def Underline(self, Dicke):

		# Linienstaerke auswaehlen
		if(Dicke == "None"): 
			Value = 0
		elif(Dicke == "Middel"): 
			Value = 1
		elif(Dicke == "Big"):
			Value = 2
		else:
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(45))
		self.UART.write(chr(Value))

		return

	# Linienstaerke einstellen:
	# Auswahl
	# - None
	# - Middel	
	# - Big
	def Underline(self, Dicke):

		# Linienstaerke auswaehlen
		if(Dicke == "None"): 
			Value = 0
		elif(Dicke == "Middel"): 
			Value = 1
		elif(Dicke == "Big"):
			Value = 2
		else:
			print("Ungueltige Liniendicke!")
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(45))
		self.UART.write(chr(Value))

		return
	
	# Textmodus setzen
	# Auswahl
	# - Inverse
	# - Updown
	# - Bold
	# - DoubleHeight
	# - DoubleWidth
	# - Deleteline	
	def PrintMode(self, Mode, Enable):

		# Modus auswaehlen
		if(Mode == "Inverse"): 
			Value = (Enable << 1)
		elif(Mode == "Updown"): 
			Value = (Enable << 2)
		elif(Mode == "Bold"): 
			Value = (Enable << 3)
		elif(Mode == "DoubleHeight"): 
			Value = (Enable << 4)
		elif(Mode == "DoubleWidth"): 
			Value = (Enable << 5)
		elif(Mode == "Deleteline"): 
			Value = (Enable << 6)
		else:
			print("Ungueltiger Modus!")
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(33))
		self.UART.write(chr(Value))

		return

	# ------------------------------------------------------------------------
	#				Linefeed
	# ------------------------------------------------------------------------	

	# Skipt eine bestimmte Anzahl Zeilen
	def Linefeed(self, Anzahl):

		if(Anzahl > 255):
			print "Anzahl der Zeilen zu hoch!"
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(100))

		for Counter in range(Anzahl):
			self.UART.write(chr(12))

		return

	# Stellt den Abstand zwischen zwei Zeilen in Punkten ein
	def SetLinespace(self, Punkte):
	
		if(Punkte > 255):
			print "Anzahl der Punkte zu hoch!"
			return -1

		self.UART.write(chr(27))
		self.UART.write(chr(51))
		self.UART.write(chr(Punkte))
	
		return

	# Setzt den Abstand zwischen zwei Zeilen auf den Default Wert (32 Punkte)
	def SetLineDefault(self):

		self.UART.write(chr(27))
		self.UART.write(chr(50))

		return

	def Tab(self):
	
		self.UART.write(chr(9))

		return
	
	# ------------------------------------------------------------------------
	#				Bitmap
	# ------------------------------------------------------------------------
	
	def PrintBitmap(self, Liste, Width, Height):

		for Row in range(Height):

			self.UART.write(chr(18))
			self.UART.write(chr(42))
			self.UART.write(chr(Height))
			self.UART.write(chr(Width))

			for Char in range(Width):	

				Daten = 255 - Liste[Row * Width + Char]
				self.UART.write(chr(Daten))

		return
	
	# ------------------------------------------------------------------------
	#				Barcode
	# ------------------------------------------------------------------------
	
	def PrintBarcode(self, Daten, Typ):

		self.UART.write(chr(29))
		self.UART.write(chr(107))
		self.UART.write(chr(Typ))

		for Counter in Daten:
			self.UART.write(Counter)

		self.UART.write(chr(00))

		return

	def BarcodeHeight(self, Hoehe):

		if(Hoehe > 255):
			print "Die Hoehe ist zu hoch!"
			return -1
	
		self.UART.write(chr(29))
		self.UART.write(chr(104))
		self.UART.write(chr(Hoehe))

		return

	def BarcodeWidth(self, Breite):

		if(Breite == "Small"):
			Value = 2
		elif(Breite == "Big"):
			Value = 3
		else:
			print "Ungueltige Angabe"
			return -1

		self.UART.write(chr(29))
		self.UART.write(chr(119))
		self.UART.write(chr(Value))

		return

	def BarcodePosition(self, Position):

		if(Position == "Oben"):
			Value = 1
		elif(Position == "Unten"):
			Value = 2
		elif(Position == "Beides"):
			Value = 3
		else:
			Value = 0

		self.UART.write(chr(29))
		self.UART.write(chr(72))
		self.UART.write(chr(Value))
		
		return
	