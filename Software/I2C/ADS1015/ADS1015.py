import smbus

class ADS1015:

	# Konstruktor
	def __init__(self, Addr):
	
		# Adresse des Baustein
		self.Adresse = 0x48 + Addr

		# Version rausfinden
		Info = open("/proc/cpuinfo", "r")

		for Zeile in Info:
			if("Revision") in Zeile:

				Zeile = Zeile.strip()
				Version = Zeile[len(Zeile) - 4:len(Zeile)]

				break

		Info.close()

		# Entsprechenden I2C-Bus festlegen
		if(Version == "0010"):
			Bus_Num = 1

		# I2C initalisieren
		self.Bus = smbus.SMBus(Bus_Num)

		return

	def Close(self):
	
		# Schnittstelle freigeben
		self.Bus.close()

		return

	def Config(self, Config):
	
		# Registerinhalt erstellen
		MSB = (Config[0] << 4) + (Config[1] << 1) + Config[2]
		LSB = (Config[3] << 5) + (Config[4] << 4) + (Config[5] << 3) + (Config[6] << 2) + Config[7]

		# Neue Daten ins Register schreiben
		self.Bus.write_i2c_block_data(self.Adresse, 1, [MSB, LSB])

		# Register auslesen
		Data = self.Bus.read_i2c_block_data(self.Adresse, 1)[:2]
		Register = (Data[0] << 8) + Data[1]

		# PGA Konfiguration filtern
		PGA = (Data[0] & 0x1C) >> 1

		# MUX Konfiguration filtern
		self.MUX = (Data[0] & 0x70) >> 4
		
		# Conversion-Modus auslesen
		self.Conv = (Data[0] & 0x01)

		# Aufloesung berechnen
		if(PGA == 0):
			self.Aufloesung = 6.144 * 2.0/4096
		elif(PGA == 1):
			self.Aufloesung = 4.096 * 2.0/4096
		elif(PGA == 2):
			self.Aufloesung = 2.048 * 2.0/4096
		elif(PGA == 3):
			self.Aufloesung = 1.024 * 2.0/4096	
		else:
			self.Aufloesung = 0.512 * 2.0/4096
	
		return Register
	
	def Read(self):

		Voltage = 0.0

		# Conversion starten, falls kein Free-Running Modus ausgewaehlt worden ist
		if(self.Conv == 1):
			Data = self.Bus.read_i2c_block_data(self.Adresse, 1)[:2]
			if(Data[0] > 0x80):
				self.Bus.write_i2c_block_data(self.Adresse, 1, Data)
			while(Data[0] < 0x80):
				self.Bus.write_i2c_block_data(self.Adresse, 1, Data)

		# Wandlungsergebnis auslesen
		Recieve = self.Bus.read_i2c_block_data(self.Adresse, 0)[:2]
		Data = ((Recieve[0] << 8) + Recieve[1]) >> 4

		# Spannung berechnen
		if(self.MUX > 3):
			Voltage = Data * self.Aufloesung
		else:
			if(Data > 2047):
				Data = (~(Data))
				Data = Data & 0x7EE
				Voltage = (-1.0) * Data * self.Aufloesung
			else:
				Data = Data & 0x7FF
				Voltage = Data * self.Aufloesung

		return Voltage