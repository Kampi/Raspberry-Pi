// Compile with: gcc /Programme/GPIO_Read.c -o /Programme/GPIO_Read
 
#include <sys/types.h>                        
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char Device_Value[] = "/sys/class/gpio/gpio17/value";
char Device_Direction[] = "/sys/class/gpio/gpio17/direction";

int main(int argc, char** argv)
{
	char Input;
	char Buffer[80];
	int fd;
	int lenght;
	
	// Pin als Eingang schalten
	fd = open(Device_Direction, O_RDWR);
	write(fd, "in", 2);
	close(fd);
	
	// Pin auslesen
	fd = open(Device_Value, O_RDWR);	
	read(fd, &Input, 1);
	close (fd);	
	
	// Die zwei überflüssigen Bits rausnehmen
	Input = Input - 48;
	
	// Status in der Konsole ausgeben
	printf("%d", Input);
	printf("\n");
	
	// Systemstring mit Übergabeparametern verbinden
	lenght = sprintf(Buffer, "/usr/bin/python /home/pi/Desktop/Autostart/Mail.py %s %d", "GPIO17:", Input);

	// E-Mail verschicken
	system(Buffer);
	
	return 0;
}