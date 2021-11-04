#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>

#define MAX 80
#define PORT 4444
#define SA struct sockaddr

void funcComunication(int var);

void funcComunication(int sockfd)
{
	char buffer[MAX];
	int n;
	while(1){
		bzero(buffer, sizeof(buffer));
		printf("[CLIENT]: ");
		n = 0;
		while ((buffer[n++] = getchar()) != '\n');
		write(sockfd, buffer, sizeof(buffer));
		bzero(buffer, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
		printf("[SERVER] : %s", buffer);
		if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servAddr, cli;

	// Creacion del Socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("[-]Socket creation failed...\n");
		exit(0);
	}
	else
		printf("[+]Socket successfully created..\n");
	bzero(&servAddr, sizeof(servAddr));

	// Definicion de Variables IP, PUERTO
	memset(&servAddr,'\0',sizeof(servAddr));
	servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	// Conexion al Socket
	if (connect(sockfd, (SA*)&servAddr, sizeof(servAddr)) != 0) {
		printf("[-]Connection with the server failed...\n");
		exit(0);
	}
	else
		printf("[+]Connected to the server..\n");

	// Función para Read / Write
	funcComunication(sockfd);

	// Se cierra el Socket
	close(sockfd);
}
