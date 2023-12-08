#ifndef CLIENT_SERVEUR_UTILS_H

//including used headers for the header file
#include <sys/socket.h> 
#include <stdbool.h>
//definition of some values                              
#define CLIENT_SERVEUR_UTILS_H
#define PARAPHARMACEUTIQUE_CODE 1
#define VEHICULE_CODE 2
#define RECETTE_GLOBALE_CODE 3
#define MAX_CLIENT 5
#define SERVER_PROXY_PORT 8080
#define SERVER_TCP_PORT 8081
#define SERVER_UDP_PORT 8082
#define SERVER_IP "127.0.0.1"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

//function declarations
bool init_proxy_server(int* sockfd,sockaddr_in* ServAddr);
bool init_client_para(int* sock_client_para,sockaddr_in* saddr);
bool connect_client(int* sockfd,int* socketDialogue,sockaddr* ClientAddr,socklen_t* longueurAdresse);
int run_proxy_server(int* dialogue_socket,int* sock_client_para);
void shutdown_proxy_server(int* sockets,size_t n);
bool connect_para_server(int* sock_client_para,sockaddr_in* server_address_para);
#endif
