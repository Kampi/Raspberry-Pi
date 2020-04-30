# PCA9685 I2C PWM controller example for the Raspberry Pi from https://www.kampis-elektroecke.de/raspberry-pi/raspberry-pi-i2c/servo/

import time
import smbus

# Device address
ADDRESS = 0x40

# Max. rotating angle of the servo from the center position
MAX_ANGLE = 45.0

# Convert the servo angle into a duty cycle
def AngleToPWM(Angle):
	if((Angle > MAX_ANGLE) or (Angle < -MAX_ANGLE)):
		print("[ERROR] Invalid servo angle!")
		Angle = 0

	return 7.5 - (Angle * 0.055)

# Open the I2C interface
Bus = smbus.SMBus(1)

# Reset the device
Bus.write_byte(0x00, 0x06)

# Set the output frequency to 50 Hz
# Prescaler = f / (4096 * f_Out)
Bus.write_byte_data(ADDRESS, 0xFE, 121)

# Read the first chip configuration byte and disable the sleep mode
Config = Bus.read_byte_data(ADDRESS, 0x00)
Config &= ~(0x01 << 0x04)
Bus.write_byte_data(ADDRESS, 0x00, Config)

# Wait a bit (at least 500 us for the sleep bit)
time.sleep(1)

print("[INFO] Configuration: {}".format(Bus.read_i2c_block_data(ADDRESS, 0x00, 0x02)))
print("[INFO] Prescaler: {}".format(Bus.read_byte_data(ADDRESS, 0xFE)))

while(True):
	# Get the output channel
	Channel = int(input("[INPUT] Output channel (0 - 15): "))

	# Get the value for the output channel
	Angle = float(input("[INPUT] Duty cycle (-{} - {}): ".format(MAX_ANGLE, MAX_ANGLE)))

	try:
		# Skip wrong input
		if((Channel > 15) or (Angle > MAX_ANGLE) or (Angle < (-MAX_ANGLE))):
			print("[ERROR] Wrong input!")
			continue

		# Convert the duty cycle into the value for the OFF register
		Angle = int(4096.0 / 100.0 * AngleToPWM(Angle))

		# Write the new value
		Bus.write_byte_data(ADDRESS, (Channel * 4) + 0x06, 0x00)
		Bus.write_byte_data(ADDRESS, (Channel * 4) + 0x07, 0x00)
		Bus.write_byte_data(ADDRESS, (Channel * 4) + 0x08, (Angle & 0xFF))
		Bus.write_byte_data(ADDRESS, (Channel * 4) + 0x09, ((Angle >> 0x08) & 0xFF))
	
	except Exception as e:
		print("[ERROR] Error: {}".format(e))