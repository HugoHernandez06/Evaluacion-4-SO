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

pthread_mutex_t keySC;

char comando[100];

struct events{
    char event_name[50];
};

struct client{
    char client_id[50];
    struct events clientsEvents;
}clients[10];

// Funciones Server
void funcExit();
void funcAdd(char *buffer);
void funcRemove(char *buffer);
void funcTrigger(char *buffer);
void funcList();
void funcAll();
void funcSaveFile(char *buffer);
void funcLoadFile(char *buffer);
// Funciones Cliente
void funcSub(char *buffer);
void unSub(char *buffer);
void funcAsk();
void funcSaveCFile(char *buffer);
void funcLoadCFile(char *buffer);

void funcComunication(int var);
/*
void *readCommands(void *arg);
void *readCommands(void *arg){
    while (1)
    {
		
        fgets(comando,sizeof(comando),stdin);
        for (int i = 0; comando[i] !='\0'; ++i)
        {
            comando[i] = tolower(comando[i]);
        }
        char *token = strtok(comando," ");
        char *action = NULL;
        char *resp;
        for (int i = 0; token!= NULL; i++)
        {
            if (i == 0)
            {
                action = token;
            }else if (i == 1)
            {
                resp = token;
            }
            token = strtok(NULL," ");
        }

        int exit = strcmp(action,"exit\n");
        int add = strcmp(action,"add");
        int remove = strcmp(action,"remove");
        int trigger = strcmp(action,"trigger");
        int list = strcmp(action,"list\n");
		int all = strcmp(action,"all\n");
		int save = strcmp(action,"save");
		int load = strcmp(action,"load");

        if (exit == 0)
        {
            funcExit();
        }else if (add == 0)
        {
            funcAdd(resp);
        }else if (remove == 0)
        {
            funcRemove(resp);
        }else if (trigger == 0)
        {
            funcTrigger(resp);
        }else if (list == 0)
        {
            funcList();
        }else if(all == 0){
			funcAll();
		}else if(save == 0){
			funcSaveFile(resp);
		}else if(load == 0){
			funcLoadFile(resp);
		}
    }
    return NULL;
}
*/
void funcComunication(int sockfd)
{
	char buffer[MAX];
	int n;

	while (1) {
		
		bzero(buffer, MAX);
		// Lectura del mensaje del Cliente se almacena en buffer
		read(sockfd, buffer, sizeof(buffer));
		// Imprime el mensaje
		printf("[CLIENT]: %s", buffer);

		char *token = strtok(buffer," ");
        char *action = NULL;
        char *resp;
        for (int i = 0; token!= NULL; i++)
        {
            if (i == 0)
            {
                action = token;
            }else if (i == 1)
            {
                resp = token;
            }
            token = strtok(NULL," ");
        }
		// Server
        int exit = strcmp(action,"exit\n");
        int add = strcmp(action,"add");
        int remove = strcmp(action,"remove");
        int trigger = strcmp(action,"trigger");
        int list = strcmp(action,"list\n");
		int all = strcmp(action,"all\n");
		int save = strcmp(action,"save");
		int load = strcmp(action,"load");
		// Cliente
		int sub = strcmp(action,"sub");
		int unsub = strcmp(action,"unsub");
		int list = strcmp(action,"list\n");
		int ask = strcmp(action,"ask\n");
		int saveC = strcmp(action,"savec");
		int loadC = strcmp(action,"loadc");

        if (exit == 0)
        {
            funcExit();
        }else if (add == 0)
        {
            funcAdd(resp);
        }else if (remove == 0)
        {
            funcRemove(resp);
        }else if (trigger == 0)
        {
            funcTrigger(resp);
        }else if (list == 0)
        {
            funcList();
        }else if(all == 0){
			funcAll();
		}else if(save == 0){
			funcSaveFile(resp);
		}else if(load == 0){
			funcLoadFile(resp);
		}else if (sub == 0)
		{
			funcSub(resp);
		}else if (unsub == 0)
		{
			funcUnsub(resp);
		}else if (list == 0)
		{
			funcListC();
		}else if (ask == 0)
		{
			funcAsk();
		}else if (saveC == 00)
		{
			funcSaveCFile(resp);
		}else if (loadC == 0)
		{
			funcLoadCFile(resp);
		}
        printf("[SERVER]: %s");
		bzero(buffer, MAX);
		n = 0;
		// Copia el mensaje en el buffer
		while ((buffer[n++] = getchar()) != '\n');
		// Envia el buffer al cliente
		write(sockfd, buffer, sizeof(buffer));
		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buffer, 4) == 0) {
			printf("[-]Server Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd, len, ret;
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
	//servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Asignación IP - Numero Puerto al Socket
	ret = bind(sockfd, (SA*)&servAddr, sizeof(servAddr));
    if (ret < 0) {
		printf("[-]Socket bind failed...\n");
		exit(0);
	}
	else
		printf("[+]Socket successfully binded..\n");

	// Socket Pasivo
	if ((listen(sockfd, 10)) != 0) {
		printf("[-]Listen failed...\n");
		exit(0);
	}
	else
		printf("[+]Server listening..\n");
	len = sizeof(cli);
	/*
	pthread_mutex_init(&keySC,NULL);
	pthread_t thread1;
	pthread_create(&thread1,NULL,&readCommands,NULL);
	pthread_join(thread1,NULL);
	*/
	while (1)
	{
		// Espera hasta que un cliente se conecta
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("[-]Server Accept failed...\n");
			exit(0);
		}
		else
			printf("[+]Server Accept the Client: %s:%d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
		// Función para Read / Write
		funcComunication(connfd);		
	}
	// Se cierra el Socket
	close(sockfd);
}
// Funciones Server
void funcExit(){
	printf("Prueba \n");
	funcTrigger("exit");
    exit(0);
}
void funcAdd(char *resp){
    printf("Prueba \n");
	for (int i = 0; i < 10; i++)
    {
        if (strcmp(clients[i].clientsEvents.event_name,"") == 0)
        {
            strcpy(clients[i].clientsEvents.event_name, resp);
            printf("Agrega %d: %s",i,clients[i].clientsEvents.event_name);
            break;
        }
    }
}
void funcRemove(char *resp){
    printf("Prueba \n");
	for (int i = 0; i < 10; i++)
    {
        if (strcmp(clients[i].clientsEvents.event_name, resp) == 0)
        {
            strcpy(clients[i].clientsEvents.event_name,"");
            printf("se elimino el evento \n" );
            break;
        }
    }
}
void funcTrigger(char *resp){
    printf("Prueba \n");
}
void funcList(){
    printf("Prueba \n");
	for (int i = 0; i < 10; i++)
    {
        printf("Eventos %d: %s\n",i,clients[i].clientsEvents.event_name);
    }
}
void funcAll(){
	printf("Prueba \n");
	for (int i = 0; i < 10; i++)
    {
        printf("Eventos %d: %s, Client: %s\n",clients[i].client_id,i,clients[i].clientsEvents.event_name);
    }
}
void funcSaveFile(char *resp){
	printf("Prueba \n");
	FILE *fp;
	fp = fopen(resp,"r+");
	if (fp = NULL)
	{
		perror("Error con el archivo\n");
	}
	for (int i = 0; i < sizeof(fp); i++)
	{
		fprintf(fp,clients[i].clientsEvents.event_name);
	}
	fclose(fp);
}
void funcLoadFile(char *resp){
	printf("Prueba \n");
	FILE *fp;
	char tmp[100];
	fp = fopen(resp,"r+");
	if (fp == NULL)
	{
		perror("Error con el archivo\n");
	}
	while (feof(fp)==0)
	{
		fgets(tmp,sizeof(tmp),fp);
		for (int i = 0; i < 10; i++)
		{
        if (strcmp(clients[i].clientsEvents.event_name,"") == 0)
			{
				strcpy(clients[i].clientsEvents.event_name, tmp);
				printf("Evento FILE: %d: %s",i,clients[i].clientsEvents.event_name);
				break;
			}
    	}
	}
	fclose(fp);
}
// Funciones Cliente
void funcSub(char *resp){
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(clients[i].clientsEvents.event_name,"") == 0)
        {
            strcpy(clients[i].clientsEvents.event_name, resp);
            printf("Agrega %d: %s",i,clients[i].clientsEvents.event_name);
            break;
        }
        //funcResponse();
    }
}
void funcUnsub(char *resp){
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(clients[i].clientsEvents.event_name, resp) == 0)
        {
            strcpy(clients[i].clientsEvents.event_name,"");
            printf("se elimino el evento \n" );
            break;
        }
        //funcResponse();
    }
}
void funcListC(){
    for (int i = 0; i < 10; i++)
    {
        printf("Eventos Cliente %d: %s\n",i,clients[i].clientsEvents.event_name);
        //funcResponse();
    }
}
void funcAsk(){
    for (int i = 0; i < 10; i++)
    {
        printf("Eventos %d: %s\n",i,clients[i].clientsEvents.event_name);
        //funcResponse();
    }
}
void funcSaveCFile(char *resp){
	printf("Prueba \n");
	FILE *fp;
	fp = fopen(resp,"r+");
	if (fp = NULL)
	{
		perror("Error con el archivo\n");
	}
	for (int i = 0; i < sizeof(fp); i++)
	{
		fprintf(fp,clients[i].clientsEvents.event_name);
	}
	fclose(fp);
}
void funcLoadCFile(char *resp){
	printf("Prueba \n");
	FILE *fp;
	char tmp[100];
	fp = fopen(resp,"r+");
	if (fp == NULL)
	{
		perror("Error con el archivo\n");
	}
	while (feof(fp)==0)
	{
		fgets(tmp,sizeof(tmp),fp);
		for (int i = 0; i < 10; i++)
		{
        if (strcmp(clients[i].clientsEvents.event_name,"") == 0)
			{
				strcpy(clients[i].clientsEvents.event_name, tmp);
				printf("Evento FILE: %d: %s",i,clients[i].clientsEvents.event_name);
				break;
			}
    	}
	}
	fclose(fp);
}
