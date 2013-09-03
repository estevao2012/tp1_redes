#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>   
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include "funcoes.c" 
 
#define MAXRCVLEN 500
#define PORTNUM 80
// #define PORTNUM 2343 

int main(int argc, char *argv[])
{
    char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
    char msg[] = "Atestecone2 8a114d6895";
    int numbytes, mysocket, i;
    struct sockaddr_in dest;
    char *hostname = "george.dcc.ufmg.br";
    char ip[100];
    char *error;
    
    hostname_to_ip(hostname , ip);
    
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }
    else
    {
        printf("the socket is ok\n");
    }
    
    memset(&dest, 0, sizeof(dest));           /* zero the struct */
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(ip);     /* set destination IP number */
    // dest.sin_addr.s_addr = inet_addr("127.0.0.1");
    dest.sin_port = htons(PORTNUM);           /* set destination port number */
    
    if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        exit(1);
    }
    else
    {
        printf("The connect() is OK\n");
    }
    
    if (send(mysocket, msg, strlen(msg), 0) == -1)//envia conexao 'A' ja com senha certa
    {
        perror("send");
        exit(1);
    }
    else
    { printf("msg send\n"); }
    
    if ((numbytes = recv(mysocket, buffer, MAXRCVLEN, 0)) == -1)//Recebe resposta
    {
        perror("recv()");
        exit(1);
    }
    
    buffer[numbytes] = '\0';

    if ( buffer[0] == 'C')
    {
 
        printf("Conexão via endereço IP:%i.%i.%i.%i\n", (unsigned int)buffer[1],(unsigned int)buffer[2],(unsigned int)buffer[3],(unsigned int)buffer[4]);
    }
    else if (buffer[0] == 'N')
    {
	char hostname_recv[100];
	int port_recv;

        for (i=2; i<buffer[1]+2; i++)
	{
		hostname_recv[i-2] = buffer[i];
	}
	port_recv = buffer[i]*256+buffer[i+1];
        printf("Conexão via nome do servidor:%s | Porta: %i", hostname_recv, port_recv);        
	printf("\n");        
    }
    else if (buffer[0] == 'S')
    {
        printf("Saida");
    }
    else if (buffer[0] == 'D')
    {
        printf("Resultado");
    }
    else if (buffer[0] == '0')
    {
        printf("OK");
    }
    else if (buffer[0] == 'E')
    {
        printf("ERRO");
    }
    printf("(%d bytes).\n", buffer[0],numbytes);
    
    close(mysocket);
    return EXIT_SUCCESS;
}
