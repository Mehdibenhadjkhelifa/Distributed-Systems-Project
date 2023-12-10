#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <unistd.h>       
#include <netinet/in.h> 
#define SERVER_TCP_PORT 8082
bool get_file_content(const char* file_path,char* buffer,size_t* buffer_size);
int  main(int argc, char* argv[]) 
{
    int  sockfd;                      
    struct sockaddr_in ServAddr;     
    struct sockaddr_in ClientAddr;    
    socklen_t   longueurAdresse; 
    char  received_buffer = -1;               
    char* buffer_to_send = "ok from tcp";
    size_t buffer_size = 12;
    ssize_t countr;                  
    int  dialogue_socket;            
    sockfd = socket(AF_INET, SOCK_STREAM,0);  
    
    if(sockfd < 0)  
    { 
        perror("Failed to create vehicule server listen socket");
        exit(-1);   
    } 
    
    printf("TCP vehicule Server listening socket created with success\n");
    longueurAdresse = sizeof(struct sockaddr_in); 
    memset(&ServAddr, 0x00, longueurAdresse); 
    ServAddr.sin_family = AF_INET; 
    ServAddr.sin_port = htons(SERVER_TCP_PORT); 
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);    
    if((bind(sockfd, (struct  sockaddr *)&ServAddr, longueurAdresse))< 0) 
    { 
        perror("bind"); 
        exit(-1); 
    } 
    printf("Listen Socket attached with success ! (%d)\n",sockfd);
    if(listen(sockfd,5)  < 0) 
    { 
        perror("listen"); 
        exit(-1); 
    } 
    printf("Waiting for proxy connection\n");
    dialogue_socket = accept(sockfd,(struct sockaddr *)&ClientAddr, &longueurAdresse); 
    if (dialogue_socket <0) 
    { 
        perror("accept"); 
        close(dialogue_socket); 
        exit(-1); 
    } 
    printf("Connected with proxy\n");
    while(1)
    {   
        received_buffer = -1;
        countr =  recv(dialogue_socket, &received_buffer,sizeof(char),0);  
        switch(countr) 
        { 
            case -1:  
                perror("recv"); 
                close(dialogue_socket); 
                exit(-1); 
            case 0: 
                fprintf(stderr,"Dialogue Socket closed by proxy\n\n"); 
                close(dialogue_socket); 
                return  0; 
            default:  
                printf("Message recue \n");
                send(dialogue_socket,buffer_to_send,buffer_size,0);
        }     
    }
    close(dialogue_socket);  
    close(sockfd); 
    return 0; 
} 

