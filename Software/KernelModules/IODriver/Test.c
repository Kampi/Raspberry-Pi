#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int fd;
	char Status;
	
	// Treiber �ffnen
	fd = open("/dev/Treiber", O_RDWR);
	
	// Pin durch Treiber setzen
	write(fd, "1", 1);
	sleep(1);
	
	// Status des Pins einlesen
	read(fd, &Status, 1);
	
	// Status ausgeben
	printf("Status: %i", Status);
	printf("\n");
	sleep(1);
	
	// Pin durch Treiber zur�cksetzen
	write(fd, "0", 1);
	sleep(1);
	
	// Status einlesen
	read(fd, &Status, 1);
	
	// Status ausgeben
	printf("Status: %i", Status);
	printf("\n");
	sleep(1);
	
	// Ung�ltige Eingabe in den Treiber schreiben
	write(fd, "fdf", 3);
	sleep(1);

	// Ger�tedatei freigeben
	close (fd);	
	
	return 0;
}