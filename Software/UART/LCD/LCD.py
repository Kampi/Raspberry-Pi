from LCD_Serial import LCD_Serial
import time

# Objekt anlegen
LCD = LCD_Serial("/dev/serial0", 9600, 20)

# Display loeschen
LCD.Clr()

# Ausgabestring
String = "Hallo"

while True:
	for I in range(1, 21 - len(String)):
		LCD.Position(3, I)
		LCD.Write(String)
		time.sleep(1)
		LCD.Clr()
	
	for I in range(21 - len(String), 0, -1):
		LCD.Position(3, I)
		LCD.Write(String)
		time.sleep(1)
		LCD.Clr()