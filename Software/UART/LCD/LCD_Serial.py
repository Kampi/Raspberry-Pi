'''
 * LCD_Serial.py
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Python-Module for the SparkFun Serial LCD Backpack.

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
'''

import serial

class LCD_Serial:
	def __init__(self, Device, Baudrate, Characters):
		self.__Device = Device
		self.__Baudrate = Baudrate
		self.__Characters = Characters
		self.__UART = serial.Serial(self.__Device, self.__Baudrate)

		if(not(self.__UART.isOpen())):
			self.__UART.open()

	def Clr(self):
		self.__UART.write(chr(0xFE))
		self.__UART.write(chr(0x01))

	def Position(self, Line, CharPosition):
		self.__UART.write(chr(0xFE))

		if(self.__Characters == 16):
			if(Line == 1):
				CharPosition = CharPosition - 1
			elif(Line == 2):
				CharPosition = CharPosition + 63
			elif(Line == 3):
				CharPosition = CharPosition + 15
			elif(Line == 4):
				CharPosition = CharPosition + 79
			else:
				print("Invalid row!")
				return -1
		elif(self.__Characters == 20):
			if(Line == 1):
				CharPosition = CharPosition - 1
			elif(self.Line == 2):
				CharPosition = CharPosition + 63
			elif(Line == 3):
				CharPosition = CharPosition + 19
			elif(Line == 4):
				CharPosition = CharPosition + 83
			else:
				print("Invalid row!")
				return -1

		else:
			print("Invalid Character!")
			return -1

		CharPosition = CharPosition + 128
		self.__UART.write(chr(CharPosition))

	def SetBrightness(self, Brightness):
		if(Brightness > 30):
			print("Invalid brightness!")
			return -1

		Brightness = 128 + self.Brightness
		self.__UART.write(chr(0x7C))
		self.__UART.write(chr(Brightness))		

	def EnableBlink(self, Enable):
		if(Enable == "Off"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0C))
		elif(Enable == "On"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0D))
		else:
			print("Invalid cursor settings!")
			return -1

	def EnableUnderline(self, Enable):
		if(Enable == "Off"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0C))
		elif(Enable == "On"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0E))
		else:
			print("Invalid cursor settings!")
			return -1

	def EnableVisual(self, Enable):
		if(Enable == "Off"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x08))
		elif(Enable == "On"):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0C))
		else:
			print("Invalid cursor settings!")
			return -1
	
	def SwitchSplash(self):
		self.__UART.write(chr(0x7C))
		self.__UART.write(chr(0x09))

	def SetCursor(self, CursorPosition):
		self.__UART.write(chr(0xFE))
		self.__UART.write(chr(128 + self.CursorPosition + 1))

	def ChangeBaud(self, Baud):		
		if(Baud == 2400):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0B))
		elif(Baud == 4800):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0C))
		elif(Baud == 9600):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0D))
		elif(Baud == 14400):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0E))
		elif(Baud == 19200):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x0F))
		elif(Baud == 38400):
			self.__UART.write(chr(0xFE))
			self.__UART.write(chr(0x10))
		else:
			print("Invalid baud rate!")
			return -1

	def Write(self, Text):
		self.__UART.write(Text)