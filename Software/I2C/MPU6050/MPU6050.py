import smbus
import time

class MPU6050_I2C:

	# Konstruktor
	# Initialisiert den Chip mit 8MHz int. Clock,
	# deaktiviert den Sleep Modus,
	# und stellt die Sensorparameter auf 250 Grad/s und +-2g ein
	def __init__(self, Bus, AD0):
	
		self.Bus = Bus
		self.Address = 0x68 + int(AD0)

		# I2C initalisieren
		self.Bus = smbus.SMBus(Bus)

		# Device reseten
		self.Reset()
		self.setClock(0)
		self.Disable_Sleep()
		self.setScale(0)
		self.setAccel(0)

		# Kurz warten bis der Reset durchgefuehrt wurde
		time.sleep(0.1)

	def Int_Enable():
	
		pass

	# Gibt die ID des Geraets zurueck
	def GetID(self):

		return self.Bus.read_byte_data(self.Address, 0x75)

	# Resetet die Register des Chips
	def Reset(self):

		# Reset ausfuehren
		self.Bus.write_byte_data(self.Address, 0x6B, 0x80)

	def Enable_Temp(self, Enable):
	
		self.Enable = Enable

		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Temperatursensor aktivieren
		Register = Register | (1 << 3)
		self.Bus.write_byte_data(self.Address, 0x6B, Register)

	def Disable_Temp(self):
	
		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Temperatursensor aktivieren
		Register = Register & 0xF7
		self.Bus.write_byte_data(self.Address, 0x6B, Register)
	
	def Enable_Cycle(self):
	
		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		if(Register < 63):

			# Cycle aktivieren
			Register = Register | (1 << 5)
			self.Bus.write_byte_data(self.Address, 0x6B, Register)

		else:

			# Fehlermeldung ausgeben
			print "Der Chip befindet sich im Sleep Modus!"

	def Disable_Cycle(self):
	
		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Cycle deaktivieren
		Register = Register & 0x5F
		self.Bus.write_byte_data(self.Address, 0x6B, Register)

	def Enable_Sleep(self):

		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Sleep Modus aktivieren
		Register = Register | (1 << 6)
		self.Bus.write_byte_data(self.Address, 0x6B, Register)
	
	def Disable_Sleep(self):
	
		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Sleep Modus deaktivieren
		Register = Register & 0x3F
		self.Bus.write_byte_data(self.Address, 0x6B, Register)

	def Read_Temp(self):
	
		# Temperaturregister auslesen
		Temp_H = self.Read_Register(0x41)
		Temp_L = self.Read_Register(0x42)

		# Temperatur berechnen
		Temp = (Temp_H << 8) | Temp_L
		Temperatur = float(Temp)/(340.0) + 36.53

		return Temperatur

	# Setzt die Taktgeschwindigkeit des Chips
	# 0 = int. 8MHz
	# 1 = PLL mit X Achsen Gyro Referenz
	# 2 = PLL mit y Achsen Gyro Referenz
	# 3 = PLL mit Z Achsen Gyro Referenz
	# 4 = PLL mit ext. 32,768kHz
	# 5 = PLL mit ext. 19,2MHz
	# 6 = -
	# 7 = Stopt den Takt
	def setClock(self, CLK):
	
		self.CLK = CLK

		# Registerinhalt auslesen
		Register = self.Read_Register(0x6B)

		# Neue Taktquelle schreiben
		Register = Register | CLK
		self.Bus.write_byte_data(self.Address, 0x6B, Register)

	# Konfiguriert das Gyroskop
	# 0 = 250 Grad/s
	# 1 = 500 Grad/s
	# 2 = 1000 Grad/s
	# 3 = 2000 Grad/s
	def setScale(self, Scale):
	
		if(Scale < 3):

			# Registerinhalt auslesen
			Register = self.Read_Register(0x1B)

			# Neue Daten schreiben
			Register = Register | (Scale << 3)       
			self.Bus.write_byte_data(self.Address, 0x1B, Register)

		else:

			# Fehlerausgabe
			print "Ungueltiger Parameter!"

	# Konfiguriert den Beschleunigungsmesser
	# 0 = 2g
	# 1 = 4g
	# 2 = 8g
	# 3 = 16g
	def setAccel(self, Scale):
	
		if(Scale < 3):

			# Registerinhalt auslesen
			Register = self.Read_Register(0x1C)

			# Neue Daten schreiben
			Register = Register | (Scale << 3)       
			self.Bus.write_byte_data(self.Address, 0x1C, Register)

		else:

			# Fehlerausgabe
			print "Ungueltiger Parameter!"

	# Liest die Werte des Beschleunigungssensors aus
	def getAccelData(self):

		Sensordata = []

		# Sensordaten auslesen
		Data = self.Bus.read_i2c_block_data(self.Address, 0x3B, 6)

		for Zaehler in range(3):

			# High und Low Wert zusammenrechnen
			Messwert = (Data[(Zaehler*2)] << 8) | Data[(Zaehler*2) + 1]

			# Zweierkomplement umrechnen
			if(Messwert > 32767):

				Messwert = Messwert - 1 
				Messwert = ~Messwert
				Messwert = Messwert & 0x7FFF
				Messwert = Messwert * (-1)
				Sensordata.append(Messwert)
			else:
				Sensordata.append(Messwert)

		return Sensordata
	
	# Liest die Werte des Lagesensors aus aus
	# Gibt die Werte als Liste zurueck [x, y, z]
	def getGyroData(self):

		Sensordata = []

		# Sensordaten auslesen
		Data = self.Bus.read_i2c_block_data(self.Address, 0x43, 6)

		for Zaehler in range(3):

			# High und Low Wert zusammenrechnen
			Messwert = (Data[(Zaehler*2)] << 8) | Data[(Zaehler*2) + 1]

			# Zweierkomplement umrechnen
			if(Messwert > 32767):

				Messwert = Messwert - 1 
				Messwert = ~Messwert
				Messwert = Messwert & 0x7FFF
				Messwert = Messwert * (-1)
				Sensordata.append(Messwert)
			else:
				Sensordata.append(Messwert)

		return Sensordata

	def Selftest(self):
	
		# Register auslesen
		Gyro = self.Read_Register(0x1B)
		Accel = self.Read_Register(0x1C)

		# Bits fuer den Selbsttest setzen
		Gyro = Gyro | 0xE0
		Accel = Accel | 0xE0
	
		# Werte in die Register schreiben
		self.Bus.write_byte_data(self.Address, 0x1B, Gyro)
		self.Bus.write_byte_data(self.Address, 0x1B, Accel)

		# Selbsttestwerte auslesen
		X = self.Read_Register(0x0D)
		Y = self.Read_Register(0x0E)
		Z = self.Read_Register(0x0F)
		R = self.Read_Register(0x10)

		# Benoetigten Bits rausfiltern
		XG_Test = X & 0x1F
		YG_Test = Y & 0x1F
		ZG_Test = Z & 0x1F

		XA_Test = X & 0xE0
		YA_Test = Y & 0xE0
		ZA_Test = Z & 0xE0
		
		XA1_Test = (R & 0x30) >> 3
		YA1_Test = (R & 0x0C) >> 3
		ZA1_Test = (R & 0x03) >> 3
		
		#XA_Test = XA_Test | XA1_Test
		#YA_Test = YA_Test | YA1_Test
		#ZA_Test = ZA_Test | ZA1_Test
		
		print XG_Test
		print YG_Test
		print ZG_Test
		print XA_Test
		print YA_Test
		print ZA_Test

	# Liest ein Register aus
	def Read_Register(self, Register):

		self.Register = Register
		return self.Bus.read_byte_data(self.Address, self.Register)