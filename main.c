#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define MAXRCVLEN 500
#define PORTNUM 2343
 
int main(int argc, char *argv[])
{
   char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   char msg[] = "Atestecone3 e5e9938b0c";
   int len, mysocket;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
 
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET; 
   // dest.sin_addr.s_addr = gethostbyname("george.dcc.ufmg.br");///; /* set destination IP number */ 
   //dest.sin_addr.s_addr = inet_addr("150.164.9.28");///; /* set destination IP number */ 
   dest.sin_addr.s_addr = inet_addr("127.0.0.1");
   dest.sin_port = htons(PORTNUM);                /* set destination port number */

   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
    
   send(mysocket, msg, strlen(msg), 0);  
 
   len = recv(mysocket, buffer, MAXRCVLEN, 0);
   buffer[len] = '\0'; 
   printf("Received %s (%d bytes).\n", buffer, len);
 
   close(mysocket);
   return EXIT_SUCCESS;
}