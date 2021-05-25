#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	char recv_buffer[8192];

	int data_fd = -1;

	memset(recv_buffer, 0, sizeof(recv_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(60002);

	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {	
		read(sockfd, recv_buffer, sizeof(recv_buffer)-1);

	}

	close(sockfd);

	data_fd = open("data_client", O_WRONLY);
	if (data_fd < 0) {
		puts("Error while open file");
		exit(EXIT_FAILURE);
	}

	write(data_fd, recv_buffer, strlen(recv_buffer));

	close(data_fd);

}
