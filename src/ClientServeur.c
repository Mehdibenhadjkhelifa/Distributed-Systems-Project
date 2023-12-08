//general headers
#include <stdlib.h>
#include <stdbool.h>
//socket specific headers
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour sockaddr_in
//my utils headers
#include "ClientServeurUtils.h"

int  main(int argc, char* argv[]) 
{
    //variables used to establish proxy to client connection
    int  sockfd;                      
    int  dialogue_socket;            
    sockaddr_in server_addr;     
    sockaddr_in client_addr;    

    //variables used to establish proxy to server_para(TCP) connection
    int  sock_client_para;
    sockaddr_in server_address_para;     

    //variables globally used
    socklen_t address_size = sizeof(sockaddr_in); 

    if(!init_proxy_server(&sockfd,&server_addr)){
        shutdown_proxy_server(&sockfd,1);
        exit(-1);
    }

    if(!init_client_para(&sock_client_para,&server_address_para)){
        int sockets_to_shutdown[] = {sockfd,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,2);
        exit(-2);
    }

    if(!connect_client(&sockfd,&dialogue_socket,(sockaddr*)&client_addr,&address_size)){
        int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,3);
        exit(-3);
    }
    if(!connect_para_server(&sock_client_para,&server_address_para)){
        int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_para};
        shutdown_proxy_server(sockets_to_shutdown,3);
        exit(-4);
    }
    
    int exit_status = run_proxy_server(&dialogue_socket, &sock_client_para);

    int sockets_to_shutdown[] = {sockfd,dialogue_socket,sock_client_para};
    shutdown_proxy_server(sockets_to_shutdown,3);

    return exit_status; 
}
