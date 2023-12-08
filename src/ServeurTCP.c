#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in
#define SERVER_TCP_PORT 8081
int  main(int argc, char* argv[]) 
{
    int  sockfd;                      
    struct sockaddr_in ServAddr;     
    struct sockaddr_in ClientAddr;    
    socklen_t   longueurAdresse; 
    char  Buffer = -1;               
    ssize_t countr;                  
    int  socketDialogue;            
    sockfd = socket(AF_INET, SOCK_STREAM,0);  
    
    if(sockfd < 0)  
    { 
        perror("Failed to create Para server listen socket");
        exit(-1);   
    } 
    
    printf("TCP Para Server listening socket created with success\n");
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
    socketDialogue = accept(sockfd,(struct sockaddr *)&ClientAddr, &longueurAdresse); 
    if (socketDialogue <0) 
    { 
        perror("accept"); 
        close(socketDialogue); 
        //close(sockfd); 
        exit(-1); 
    } 
    printf("Connected with proxy\n");
    while(1)
    {   
        Buffer = -1;
        countr =  recv(socketDialogue, &Buffer,sizeof(char),0);  
        switch(countr) 
        { 
            case -1:  
                perror("recv"); 
                close(socketDialogue); 
                exit(-1); 
            case 0: 
                fprintf(stderr,"Dialogue Socket closed by proxy\n\n"); 
                close(socketDialogue); 
                return  0; 
            default:  
                printf("Message recue \n");
        }     
    }
    close(socketDialogue);  
    close(sockfd); 
    return 0; 
} 
