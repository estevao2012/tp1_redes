#define MAXRCVLEN 500

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++){
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}

char *conecta_ip_recv(char *ip,unsigned short portnum, char *msg)
{
    char resposta[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
    int numbytes, mysocket;
    struct sockaddr_in dest;

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
    dest.sin_port = htons(portnum);           

    if (connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
        printf("ERRO ip: %s",ip);
        printf(" port: %i\n",portnum);
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
   
    close(mysocket);
    
    return resposta;
}
