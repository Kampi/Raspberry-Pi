// Compile with: gcc /Programme/UART_Empfangen.c -o /Programme/UART_Empfangen

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B9600

char UART[]= "/dev/serial0";
char FILEDEVICE[]= "/tmp/UART_Empfangen.txt";
char Message[255]= "";
int Length;
int File;
struct termios newtio={};

void WriteFile(void)
{ 
	int Counter;    
	int Temp;

	Temp = open(FILEDEVICE, O_WRONLY | O_CREAT | O_APPEND);
	
	write(Temp, Message, Length);
	close(Temp);
	system("chmod 644 /tmp/UART_Empfangen.txt");
  
	for(Counter = 0; Counter < Length; Counter++)
	{
        Message[Counter] = 0;
    }

	Length = 0;
}

unsigned char Empfangen(void)
{
    int res;
    unsigned char Buffer;
    res = read(File, &Buffer, 1);

    return Buffer;
}

int UART_Init(void)	
{
    File = open(UART, O_RDONLY | O_NOCTTY);
    if(File < 0)
    {
        printf("Can not open serial0! %s\n", UART);
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
    char Zeichen;

    UART_Init();
    while(1)
    {
        Character = Empfangen();
		if((Character == 13))
		{
			Message[Length++] = 0x0D;
			WriteFile();
		}
		else if(Character > 13)
		{
			Message[Length++] = Character;
			if(Length > 254)
			{
				WriteFile();
			}
		}

		close(File);
	}

    return 0;
}