// Compile with: gcc /Programme/UART_Senden.c -o /Programme/UART_Senden

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B9600

char MODEMDEVICE[]= "/dev/ttyAMA0"; 
struct termios newtio={};
int File;

int UART_Init(void)
{ 
    File = open(MODEMDEVICE, O_WRONLY | O_NOCTTY);
    if(File < 0)
    {
        printf("Fehler beim öffnen von %s\n", MODEMDEVICE);
        exit(-1);
    }

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;
    tcflush(File, TCIFLUSH);
    tcsetattr(File, TCSANOW, &newtio);

    return File;
}


int main(int argc, char** argv) 
{
	char LineEnd[] = "\r\n";
	int res;

    UART_Init();
	if(argc >= 1)                                                     
	{
		res = write(File, argv[1], strlen(argv[1]));
		write(File, LineEnd, 2);
	}
	
    close(File);
    return 0;
}