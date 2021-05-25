/**
 * @file testebbchar.c
 * @brief A Linux user-space program that communicates with the ebbchar.c LKM. It
 * passes a string to the LKM and reads the response from the LKM. For this 
 * example to work the device must be called /dev/ebbchar.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH]; // The receive buffer from the LKM

int main(void)
{
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];

	printf("Starting device test code example...\n");
	fd = open("/dev/ebbchar", O_RDWR); // Open device with read/write access
	if (fd < 0) {
		perror("Failed to open the device...");
		return errno;
	}
	printf("Type in a short string to sen to the kernel module:\n");
	scanf("%[^\n]%*c", stringToSend); // Read in a string (with spaces)
	printf("Writing message to the deivce [%s].\n", stringToSend);
	ret = write(fd, stringToSend, strlen(stringToSend)); // send string to LKM
	if (ret < 0) {
		perror("Failed to write the message to the device.");
		return errno;
	}
	printf("Press ENTER to read back from the device...\n");
	getchar();

	printf("Reading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH); // Read response from the LKM
	if (ret < 0) {
		perror("Failed to read message from the device.");
		return errno;
	}
	printf("The received message is: [%s]\n", receive);
	printf("End of the program");

	return 0;
}
