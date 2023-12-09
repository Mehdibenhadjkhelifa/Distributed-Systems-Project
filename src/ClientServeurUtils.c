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
#include "bits/sockaddr.h"

//Initialize server
//return false if failed to init the server(proxy)
//otherwiser returns true
bool init_proxy_server(int* sockfd,sockaddr_in* server_address){
    *sockfd = socket(AF_INET, SOCK_STREAM,0);  
    if(*sockfd < 0)  
    { 
        perror("Failed to create proxy server listen socket");
        return false;
    } 
    printf("Proxy Server listening socket created with success\n");
    memset(server_address, 0x00, sizeof(sockaddr_in)); 
    server_address->sin_family = AF_INET; 
    server_address->sin_port = htons(SERVER_PROXY_PORT); 
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);    
    if((bind(*sockfd,(sockaddr *)server_address, sizeof(sockaddr_in))< 0)) 
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
bool init_client_vehicule(int* socket_client_para,sockaddr_in* server_address_vehicule){
    
    *socket_client_para = socket(AF_INET,SOCK_STREAM,0); 
    if(*socket_client_para< 0)  
    { 
        perror("Failed to create client vehicule socket");
        return false;
    } 
    memset(server_address_vehicule, 0x00, sizeof(sockaddr_in)); 
    server_address_vehicule->sin_family = AF_INET;
    server_address_vehicule->sin_port = htons(SERVER_TCP_PORT);
    server_address_vehicule->sin_addr.s_addr = inet_addr(SERVER_IP);
    printf("Successfully created and initialized socket for vehicule server\n");

    return true;
}
bool init_client_para(int *sock_client_para, sockaddr_in *server_address_para,hostent* para_host){
    *sock_client_para= socket(AF_INET,SOCK_DGRAM,0); 
    if(*sock_client_para< 0)  
    { 
        perror("Failed to create client para socket");
        return false;
    } 
    para_host = gethostbyname(SERVER_IP);  
    if(para_host==NULL) {  
        perror("gethostbyname failed");
        return false;
    }  
    memset(server_address_para, 0x00, sizeof(sockaddr_in)); 
    server_address_para->sin_family = (sa_family_t)para_host->h_addrtype;
    memcpy((char *) &server_address_para->sin_addr.s_addr, para_host->h_addr_list[0], (unsigned long)para_host->h_length); 
    server_address_para->sin_port = htons(SERVER_UDP_PORT);
    memset(&server_address_para->sin_zero,0,8);      
    server_address_para->sin_addr.s_addr = inet_addr(SERVER_IP);
    printf("Successfully created and initialized socket for para server\n");
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
bool connect_vehicule_server(int* sock_client_vehicule,sockaddr_in* server_address_vehicule){
    printf("Connecting to Vehicule Server\n");
    while (connect(*sock_client_vehicule,(sockaddr *)server_address_vehicule,sizeof(*server_address_vehicule)) == -1) sleep(1);
    printf("Connected to vehicule Server successfully\n");
    return true;
}
int run_proxy_server(int* dialogue_socket,int* sock_client_vehicule,int* sock_client_para,sockaddr_in* server_address_para){

    char  Buffer = -1;               
    ssize_t countr;                  
    socklen_t address_length = sizeof(sockaddr_in); 
    while(1)
    {   
        Buffer = -1;
        countr =  recv(*dialogue_socket, &Buffer,sizeof(char),0);  
        switch(countr) 
        { 
            case -1:  
                fprintf(stderr,"recv failed from socket %d/\n",*dialogue_socket); 
                return -6;
            case 0: 
                fprintf(stderr,"Dialogue Socket closed by client\n\n"); 
                return 0;
            default:  
                printf("Message recue \n");
                switch (Buffer){
                    case PARAPHARMACEUTIQUE_CODE:
                        printf("Recue code PARA\n");
                        //TODO : communication avec server 1 pour traitement para
                        ssize_t counts = sendto(*sock_client_para, &Buffer,sizeof(char),0,(sockaddr *)server_address_para,address_length);
                        switch(counts) 
                        { 
                            case -1:
                                perror("sendto failed"); 
                                return -7;
                            case 0: 
                                fprintf(stderr, "No data has been sent!n\n"); 
                                return 0; 
                            default : 
                                if(counts != 1) 
                                    fprintf(stderr, "An error has occured while sending data!\n\n"); 
                                else{
                                    char received_buffer[MAX_BUFFER_LENGTH];
                                    memset(received_buffer,0x00,MAX_BUFFER_LENGTH);
                                    printf("Message: %d sent successfully (%zd octets)\n\n", Buffer, counts); 
                                    printf("waiting for response\n");
                                    recvfrom(*sock_client_para,received_buffer,MAX_BUFFER_LENGTH,0,(sockaddr*)server_address_para,&address_length);
                                    printf("received buffer : \n %s \n",received_buffer);
                                    send(*dialogue_socket,received_buffer,strlen(received_buffer) + 1,0);
                                } 
                        } 
                        break;
                    case VEHICULE_CODE:
                        printf("Recue code VEHICULE\n");
                        //TODO : communication avec server 2 pour traitement Vehicule 
                        send(*sock_client_vehicule,&Buffer,sizeof(char),0);
                        printf("Message sent with success\n");
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
