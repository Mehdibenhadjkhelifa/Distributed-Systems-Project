//general headers
#include <stdlib.h>
#include <stdbool.h>
//socket specific headers
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <unistd.h>       
#include <netinet/in.h> 
//my utils headers
#include "ClientServeurUtils.h"

int  main(int argc, char* argv[]) 
{
    //variables used to establish proxy to client connection
    int  sockfd;                      
    int  dialogue_socket;            
    sockaddr_in server_addr;     
    sockaddr_in client_addr;    

    //variables used to establish proxy to server_para(UDP) connection
    int  sock_client_para;
    sockaddr_in server_address_para;     
    hostent para_host;  

    //variables used to establish proxy to server_vehicule(TCP) connection
    int  sock_client_vehicule;
    sockaddr_in server_address_vehicule;     

    //variables globally used
    socklen_t address_size = sizeof(sockaddr_in); 

    if(!init_proxy_server(&sockfd,&server_addr)){
        shutdown_proxy_server(&sockfd,1);
        exit(-1);
    }

    if(!init_client_vehicule(&sock_client_vehicule,&server_address_vehicule)){
        int sockets_to_shutdown[] = {sockfd,sock_client_vehicule};
        shutdown_proxy_server(sockets_to_shutdown,2);
        exit(-2);
    }
    if(!init_client_para(&sock_client_para,&server_address_para,&para_host)){
        int sockets_to_shutdown[] = {sockfd,sock_client_vehicule,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,3);
        exit(-3);
    }

    if(!connect_client(&sockfd,&dialogue_socket,(sockaddr*)&client_addr,&address_size)){
        int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_vehicule,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,4);
        exit(-4);
    }
    if(!connect_vehicule_server(&sock_client_vehicule,&server_address_vehicule)){
        int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_vehicule,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,4);
        exit(-5);
    }
    
    int exit_status = run_proxy_server(&dialogue_socket, &sock_client_vehicule,&sock_client_para,&server_address_para);

    int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_vehicule,sock_client_para};
    shutdown_proxy_server(sockets_to_shutdown,4);

    return exit_status; 
}
