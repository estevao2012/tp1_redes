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
    char msg[] = "Atestecone2 8a114d6895";
    char *hostname = "george.dcc.ufmg.br";
    char ip[100]; 
    char ip_recv[100];  
    int  i; 
    unsigned short port_recv;     
    //---------  Variaveis --------//
    
    hostname_to_ip(hostname , ip); 
    port_recv = PORTNUM;
    while(1){
        printf("Conecta a %s:%i\n",ip,port_recv );
        strcpy(resposta,conecta_ip_recv(ip,port_recv,msg));
        sleep(1);
        // printf("Valor recebido : %s\n", resposta);
        if ( resposta[0] == 'C'){    //tratando mensagem recebida 
                
        	sprintf(ip_recv,"%i.%i.%i.%i", (unsigned char)resposta[1],(unsigned char)resposta[2],(unsigned char)resposta[3],(unsigned char)resposta[4]);

            printf("retorno via endereço IP\n");
        	port_recv = (unsigned char)resposta[5]*256+(unsigned char)resposta[6];
            strcpy(ip,ip_recv);
            // printf("Porta:%i\n",port_recv); 
            // printf("%s\n",msg );
            // break;
            sleep(1);

        }else if (resposta[0] == 'N'){

            char hostname_recv[100];
            int tamanho_string = resposta[1];
            
            // printf("tamanho string, %i\n",tamanho_string);

            for (i=2; tamanho_string > i-2; i++)
                hostname_recv[i-2] = resposta[i];
            
            hostname_recv[i-2] = '\0';

            port_recv = (unsigned char)resposta[i]*256+(unsigned char)resposta[i+1];
            hostname_to_ip(hostname_recv , ip);
            // printf("Nome do servidor: %s , %s | Porta: %i\n", hostname_recv,ip, port_recv);        
            printf("Retorno por Nome do servidor: %s \n", hostname_recv);        
            // printf("\n");  
            sleep(1);      
            // break; 

            

        }else if (resposta[0] == 'S'){	

            printf("Saida: O cliente deve fechar a conexão e informar ao servidor a ultima conexao realizada");
            printf("A mensagem de resultado comeca com o caracter D seguido de endereco ip e porta em hexadecimal");       
            break;

        }
        else if (resposta[0] == '0'){

            printf("mensagem resultado esta correta\n");       
            break;

        }
        else if (resposta[0] == 'E'){

            printf("ERRO : %s\n",resposta);       
            break;

        } 
    }
    // printf("Resposta: %s\n",conecta_ip_recv(ip, port_recv, msg));
 
    return EXIT_SUCCESS;
}
