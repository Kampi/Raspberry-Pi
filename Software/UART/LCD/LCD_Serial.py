# Klasse fuer das serielle LCD Modul "SerLCD" von Sparkfun
# www.kampis-elektroecke.de

import serial

class LCD_Serial:
	
	# Konstruktor
	def __init__(self, device, baudrate, Zeichen):
	
		self.device = device
		self.baudrate = baudrate
		self.Zeichen = Zeichen

		self.UART = serial.Serial(self.device, self.baudrate)

		# Schnittstelle oeffnen
		if(not(self.UART.isOpen())):
			self.UART.open()

	def Clr(self):

		# Commandocode
		self.UART.write(chr(0xFE))
	
		# Display loeschen
		self.UART.write(chr(0x01))

	# Legt die Position des Cursors fest
	# Line = 1 - 4
	# Position = 1 - 20
	def Position(self, Line, CharPosition):

		self.Line = Line
		self.CharPosition = CharPosition

		# Commandocode
		self.UART.write(chr(0xFE))
	
		# Auswahl der Zeile
		if(self.Zeichen == 16):

			if(self.Line == 1):
				self.CharPosition = self.CharPosition - 1
			elif(self.Line == 2):
				self.CharPosition = self.CharPosition + 63
			elif(self.Line == 3):
				self.CharPosition = self.CharPosition + 15
			elif(self.Line == 4):
				self.CharPosition = self.CharPosition + 79
			else:
				print("Ungueltige Zeilenangabe!")
				return -1

		elif(self.Zeichen == 20):

			if(self.Line == 1):
				self.CharPosition = self.CharPosition - 1
			elif(self.Line == 2):
				self.CharPosition = self.CharPosition + 63
			elif(self.Line == 3):
				self.CharPosition = self.CharPosition + 19
			elif(self.Line == 4):
				self.CharPosition = self.CharPosition + 83
			else:
				print("Ungueltige Zeilenangabe!")
				return -1

		else:
			print("Ungueltige Zeichenangabe!")
			return -1

		# Zeile einstellen
		self.CharPosition = self.CharPosition + 128
		self.UART.write(chr(self.CharPosition))

	# Einstellen der Helligkeit in 30 Stufen von 1-30
	def SetBrightness(self, Brightness):
	
		self.Brightness = Brightness

		# Wert pruefen
		if(self.Brightness > 30):
			print("Unguelige Helligkeit")
			return -1

		# Helligkeitswert berechnen
		self.Brightness = 128 + self.Brightness
	
		# Wert senden
		self.UART.write(chr(0x7C))
		self.UART.write(chr(self.Brightness))		

	# Blinkenden Cursor aktivieren
	# On = Aktiviert
	# Off = Deaktiviert
	def EnableBlink(self, Enable):
	
		self.Enable = Enable

		# Cursor aktivieren?
		if(self.Enable == "Off"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0C))
		elif(self.Enable == "On"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0D))
		else:
			print("Ungueltige Cursoreinstelllung!")
			return -1

	# Underline aktivieren
	# On = Aktiviert
	# Off = Deaktiviert	
	def EnableUnderline(self, Enable):

		self.Enable = Enable

		# Unterstrich aktivieren?
		if(self.Enable == "Off"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0C))
		elif(self.Enable == "On"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0E))
		else:
			print("Ungueltige Cursoreinstelllung!")
			return -1
	
	# Display deaktivieren
	# On = Aktiviert
	# Off = Deaktiviert	
	def EnableVisual(self, Enable):

		self.Enable = Enable
		
		# Display aktivieren / deaktivieren?
		if(self.Enable == "Off"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x08))
		elif(self.Enable == "On"):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0C))
		else:
			print("Ungueltige Cursoreinstelllung!")
			return -1
	
	def SwitchSplash(self):

		self.UART.write(chr(0x7C))
		self.UART.write(chr(0x09))

	# Setzt den Cursor an eine bestimmte Position auf dem LCD
	def SetCursor(self, CursorPosition):
	
		self.CursorPosition = 128 + self.CursorPosition + 1
	
		# Daten senden
		self.UART.write(chr(0xFE))
		self.UART.write(chr(self.CursorPosition))
	
	# Aendert die Baudrate des Modules
	def ChangeBaud(self, Baud):
	
		self.Baud = Baud
		
		if(self.Baud == 2400):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0B))
		elif(self.Baud == 4800):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0C))
		elif(self.Baud == 9600):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0D))
		elif(self.Baud == 14400):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0E))
		elif(self.Baud == 19200):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x0F))
		elif(self.Baud == 38400):
			self.UART.write(chr(0xFE))
			self.UART.write(chr(0x10))
		else:
			print("Ungueltige Baudrate!")
			return -1

	# Text auf das LCD schreiben
	def Write(self, Text):

		self.Text = Text
		self.UART.write(self.Text)