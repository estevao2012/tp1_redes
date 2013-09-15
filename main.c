#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>   
#include <errno.h> 
#include <netdb.h>
#include "funcoes.c" 
 
#define MAXRCVLEN 500
#define PORTNUM 80

int main(int argc, char *argv[])
{
    //---------  Variaveis --------//
    char resposta[MAXRCVLEN + 1]; 
    char tmp_resposta[MAXRCVLEN + 1];
    char msg[] = "Atestecone2 8a114d6895";
    char *hostname = "george.dcc.ufmg.br";
    char ip[16]; 
    char ip_recv[16]; 
    int  i,j=0,h=0,tmp_ip; 
    unsigned short port_recv;
    int indice_porta;
    in_addr_t addressIp;
    //---------  Variaveis --------// 

    int numbytes, mysocket;
    struct sockaddr_in dest;
    hostname_to_ip(hostname , ip); 
    port_recv = PORTNUM;

    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }
    else
    {
        // printf("the socket is ok\n");
    }

    memset(&dest, 0, sizeof(dest));           
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(ip);     
    dest.sin_port = htons(port_recv);           

    if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
        printf("ERRO ip: %s",ip);
        printf(" port: %i\n",port_recv);
        perror("connect()");
        exit(1);
    }else{
        // printf("The connect() is OK\n");
    }
    
    if (send(mysocket, msg, strlen(msg), 0) == -1){
        perror("send");
        exit(1);
    }else{ 
        // printf("msg sent\n"); 
    }

    if ((numbytes = recv(mysocket, resposta, MAXRCVLEN, 0)) == -1){
        perror("recv()");
        exit(1);
    }

    resposta[numbytes] = '\0';
   
    
    
    while(1){  
        if ( resposta[0] == 'C'){    //tratando mensagem recebida 
            strcpy(tmp_resposta,resposta);                

        	sprintf(ip_recv,"%i.%i.%i.%i", (unsigned char)resposta[1],(unsigned char)resposta[2],(unsigned char)resposta[3],(unsigned char)resposta[4]);

            printf("retorno via endereço IP\n");
        	port_recv = (unsigned char)resposta[5]*256+(unsigned char)resposta[6];
            strcpy(ip,ip_recv); 

        }else if (resposta[0] == 'N'){
            strcpy(tmp_resposta,resposta);

            char hostname_recv[100];
            int tamanho_string = resposta[1]; 

            for (i=2; tamanho_string > i-2; i++)
                hostname_recv[i-2] = resposta[i];
            
            hostname_recv[i-2] = '\0';

            indice_porta = i;

            port_recv = (unsigned char)resposta[i]*256+(unsigned char)resposta[i+1];
            hostname_to_ip(hostname_recv , ip);
            printf("Retorno por Nome do servidor: %s \n", hostname_recv);    

        }else if (resposta[0] == 'S'){	

            // printf("Saida: O cliente deve fechar a conexão e informar ao servidor a ultima conexao realizada");
            // printf("A mensagem de resultado comeca com o caracter D seguido de endereco ip e porta em hexadecimal\n");       
            // memset(msg ,"",sizeof(msg)); 
            //Convert Ip to hexaDecimal
            char final[7];   
            char tmp_hostnam[MAXRCVLEN];
            if(tmp_resposta[0] == 'C'){
                strcpy(final,tmp_resposta);
                final[0] = 'D';
            }else if(tmp_resposta[0] == 'N'){
                sprintf(final,"D%02X%02X%02X",(unsigned char)inet_addr(ip),tmp_resposta[indice_porta],tmp_resposta[indice_porta+1]);
            }
            // printf("%s\n",final );
           

            if (send(mysocket, final, strlen(final), 0) == -1){
                perror("send");
                exit(1);
            }else{ 
                printf("msg sent\n"); 
            }
            if ((numbytes = recv(mysocket, resposta, MAXRCVLEN, 0)) == -1){
                perror("recv()");
                exit(1);
            }

            resposta[numbytes] = '\0';
            printf("%s\n", resposta );
            // break;

        }

        if (resposta[0] == 'O'){

            printf("mensagem resultado esta correta\n");       
            break;

        }
        else if (resposta[0] == 'E'){

            // printf("ERRO : %s\n",resposta);       
            break;

        } 
        else{
            printf("%i - Conecta a %s:%i\n",j,ip,port_recv );
            strcpy(resposta,conecta_ip_recv(ip,port_recv,msg));
            j++;
        }
    }
   
    close(mysocket);
    
 
    return EXIT_SUCCESS;
}
