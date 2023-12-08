#include <stdio.h>      // pour printf() et fprintf()
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <stdlib.h>     // pour atoi(), exit() 
#include <string.h>     // pour memset(), strcpy()  
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in

#define SERVER_UDP_PORT 8082
int  main(int argc, char * argv[]) 
{ 
    int  sockfd;                   // descripteur de socket 
    struct sockaddr_in ServAddr;   // @ du serveur
    struct sockaddr_in ClientAddr; // @ du client
    unsigned int longueurAdresse = sizeof(struct   sockaddr_in); 
    char  Buffer;// Buffer de réception
    sockfd = socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd < 0)  /*** échec ? ***/
    { 
        perror("Probleme de creation de socket\n ");  /***  Affiche le message d ’erreur ***/  
        exit(-1);    /*** On sort en indiquant un code erreur ***/
    } 
    printf("Socket creee avec succes!  (%d)\n",sockfd); 
    memset(&ServAddr, 0x00, longueurAdresse); 
    ServAddr.sin_family = AF_INET; 
    ServAddr.sin_addr.s_addr =  htonl(INADDR_ANY);    
    ServAddr.sin_port = htons(SERVER_UDP_PORT);    
    if((bind(sockfd,(struct  sockaddr *)&ServAddr, longueurAdresse)) <  0) 
    { 
        perror("bind"); 
        exit(-2); 
    } 
    printf("Socket attachee avec succes!\n"); 
    while(1){
        memset(&ClientAddr, 0x00, longueurAdresse); 
        ssize_t countr = recvfrom(sockfd, &Buffer,  sizeof(Buffer), 0, (struct  sockaddr *)&ClientAddr, &longueurAdresse); 
        switch(countr) 
        { 
            case  -1  : 
                perror("recvfrom failed"); 
                close(sockfd); 
                exit(-3); 
            case  0 : 
                fprintf(stderr, "Aucune donnée n a été reçue!\n\n"); 
                close(sockfd); 
                return   0; 
            default :  /***  réception   de  n octets   ***/
                printf("Message %d recu avec succes (%zd  octets)\n\n", Buffer, countr); 
        } 
    }
    /* Fermeture de socket */
    close(sockfd); 

   return   0; 
} 
