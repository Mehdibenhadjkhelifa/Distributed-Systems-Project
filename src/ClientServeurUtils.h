#ifndef CLIENT_SERVEUR_UTILS_H
#define CLIENT_SERVEUR_UTILS_H
//including used headers for the header file
#include <netdb.h>
#include <sys/socket.h> 
#include <stdbool.h>
//definition of some values                              
#define PARAPHARMACEUTIQUE_CODE 1
#define VEHICULE_CODE 2
#define RECETTE_GLOBALE_CODE 3
#define MAX_CLIENT 5
#define SERVER_PROXY_PORT 8080
#define SERVER_UDP_PORT 8081
#define SERVER_TCP_PORT 8082
#define SERVER_IP "127.0.0.1"
#define MAX_BUFFER_LENGTH 5000
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
//function declarations
bool init_proxy_server(int* sockfd,sockaddr_in* server_address);
bool init_client_vehicule(int* sock_client_vehicule,sockaddr_in* server_address_vehicule);
bool init_client_para(int* sock_client_para,sockaddr_in* server_address_para,hostent* para_host);
bool connect_client(int* sockfd,int* socketDialogue,sockaddr* ClientAddr,socklen_t* longueurAdresse);
bool connect_vehicule_server(int* sock_client_vehicule,sockaddr_in* server_address_vehicule);
int run_proxy_server(int* dialogue_socket,int* sock_client_vehicule,int* sock_client_para,sockaddr_in* server_address_para);
void shutdown_proxy_server(int* sockets,size_t n);
#endif
