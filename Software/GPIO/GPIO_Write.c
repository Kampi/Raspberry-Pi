// Compile with: gcc /Programme/GPIO_Write.c -o /Programme/GPIO_Write
 
#include <sys/types.h>                        
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char GPIO[3][30] = {"/sys/class/gpio/gpio9/value",
					"/sys/class/gpio/gpio10/value", 
					"/sys/class/gpio/gpio11/value"}; 

int main(int argc, char** argv)
{
	int fd;
	int res;

	printf("Anzahl Argumente: %d\n", argc);

	for(int i = 0x00; i < argc; i++)
	{
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	if(argc >= 0x01)
	{
		for(int i = 0x01; i < argc; i++)
		{
			fd = open(GPIO[i - 1], O_WRONLY | O_NOCTTY);
			res = write(fd, argv[i], 1);
			close(fd);
		}
	}

	return 0;
}