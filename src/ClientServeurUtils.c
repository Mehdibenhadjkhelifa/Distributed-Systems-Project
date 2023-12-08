//general utility headers
#include <stdio.h>      // pour printf() et fprintf()
#include <stdlib.h>
#include <string.h>     // pour memset(), strcpy()  
#include <stdbool.h>
//socket specific headers
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour sockaddr_in
//My utily headers
#include "ClientServeurUtils.h"

//Initialize server
//return false if failed to init the server(proxy)
//otherwiser returns true
bool init_proxy_server(int* sockfd,sockaddr_in* ServAddr){
    *sockfd = socket(AF_INET, SOCK_STREAM,0);  
    if(*sockfd < 0)  
    { 
        perror("Failed to create proxy server listen socket");
        return false;
    } 
    printf("Proxy Server listening socket created with success\n");
    memset(ServAddr, 0x00, sizeof(sockaddr_in)); 
    ServAddr->sin_family = AF_INET; 
    ServAddr->sin_port = htons(SERVER_PROXY_PORT); 
    ServAddr->sin_addr.s_addr = htonl(INADDR_ANY);    
    if((bind(*sockfd,(sockaddr *)ServAddr, sizeof(sockaddr_in))< 0)) 
    { 
        perror("bind failed"); 
        return false;
    } 
    printf("Listen Socket attached with success ! (%d)\n",*sockfd);
    if(listen(*sockfd,MAX_CLIENT)  < 0) 
    { 
        perror("listen failed"); 
        return false;
    } 
    return true;
}

//initialize client to communicate with para server
bool init_client_para(int* socket_client_para,sockaddr_in* saddr){
    
    *socket_client_para = socket(AF_INET,SOCK_STREAM,0); 
    if(*socket_client_para< 0)  
    { 
        perror("Failed to create client para socket");
        return false;
    } 
    memset(saddr, 0x00, sizeof(sockaddr_in)); 
    saddr->sin_family = AF_INET;
    saddr->sin_port = htons(SERVER_TCP_PORT);
    saddr->sin_addr.s_addr = inet_addr(SERVER_IP);
    return true;
}

//establish client proxy connection
bool connect_client(int* sockfd,int* dialogue_socket,sockaddr* client_addr,socklen_t* address_size){
    
    printf("Waiting for client connection\n");
    *dialogue_socket = accept(*sockfd,(sockaddr *)client_addr, address_size); 
    if (dialogue_socket <0) 
    { 
        perror("Accept failed"); 
        return false;
    } 
    printf("Connected with client\n");
    return true;
}
bool connect_para_server(int* sock_client_para,sockaddr_in* server_address_para){
    printf("Connecting to Para Server\n");
    while (connect(*sock_client_para,(sockaddr *)server_address_para,sizeof(*server_address_para)) == -1) sleep(1);
    printf("Connected to Para Server successfully\n");
    return true;
}
int run_proxy_server(int* dialogue_socket,int* sock_client_para){

    char  Buffer = -1;               
    ssize_t countr;                  
    while(1)
    {   
        Buffer = -1;
        countr =  recv(*dialogue_socket, &Buffer,sizeof(char),0);  
        switch(countr) 
        { 
            case -1:  
                fprintf(stderr,"recv failed from socket %d/\n",*dialogue_socket); 
                return -5;
            case 0: 
                fprintf(stderr,"Dialogue Socket closed by client\n\n"); 
                return 0;
            default:  
                printf("Message recue \n");
                switch (Buffer){
                    case PARAPHARMACEUTIQUE_CODE:
                        printf("Recue code PARA\n");
                        //TODO : communication avec server 1 pour traitement para
                        send(*sock_client_para,&Buffer,sizeof(char),0);
                        printf("Message sent with success\n");
                        break;
                    case VEHICULE_CODE:
                        printf("Recue code VEHICULE\n");
                        //TODO : communication avec server 2 pour traitement Vehicule 
                        break;
                    case RECETTE_GLOBALE_CODE:
                        printf("Recue code RECETTE\n");
                        //TODO : communication avec les deux server pour calcule de recette 
                        break;
                    default:
                        perror("Error in the received buffer from client");
                        break;
                }
                break;
        }     
    }
    return 0;
}
void shutdown_proxy_server(int* sockets,size_t n){
    printf("Shutting down the proxy server\n");
    for(size_t i = 0; i < n ; i++){
        close(sockets[i]);
    }
}
