/*
	Mati�re: Syst�mes et Applications R�partis (SAR)                                 
	Facult�: FST
    Section: IF4
    A.U: 2014/2015

	S�rie n�1 (Sockets): 
	Exercice 1
    Une application mettant en �uvre 2 machines diff�rentes 
    (serveur et client) qui consiste � ce que le serveur fasse 
    un �cho du message envoy� par le client :

	1. Ecrire les deux programmes ClientEcho.c et ServerEcho.c, 
       sachant que l�adresse et le port du serveur et le message �
       envoyer doivent �tre pass�s en ligne de commande.  

	usage:	
		Compilation: gcc ServeurEcho.c -o ServeurEcho
		Ex�cution: ./ServeurEcho port

	Thouraya Louati. 
*/
#include <stdio.h>      // pour printf() et fprintf()
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <stdlib.h>     // pour atoi(), exit() 
#include <string.h>     // pour memset(), strcpy()  
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in

#define MAX 100        //longueur du Buffer

int  main(int argc, char * argv[]) 
{ 
   int  sockfd;                   // descripteur de socket 
   struct sockaddr_in ServAddr;   // @ du serveur
   struct sockaddr_in ClientAddr; // @ du client
   unsigned int longueurAdresse;  // taille
   char  Buffer[MAX];             // Buffer de r�ception
   unsigned short ServPort;       // port du serveur
   int  countr;                   // count receive: longueur du message re�u
   
   /*** Le port du serveur doit �tre pass� en ligne de commande ***/
   if(argc < 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}
   /*** Conversion du port du serveur d'une cha�ne de caract�res en nombres ***/
   ServPort=atoi(argv[1]);
   /* Cr�ation de socket */
	/*** Description: socket() cr�e un point de communication, et renvoie un descripteur ***/
    sockfd = socket(AF_INET,SOCK_DGRAM,0); 
    /*** 0  indique que l�on utilisera le protocole par d�faut associ� � SOCK_DGRAM soit  UDP  ***/
	/*** Valeur renvoy�e: Cet appel syst�me renvoie un descripteur r�f�ren�ant la socket cr��e s�il r�ussit. 
         S�il �choue, il renvoie -1 et perror affiche le message d'erreur ***/
    if(sockfd < 0)  /*** �chec ? ***/
    { 
       perror("Probleme de creation de socket\n ");  /***  Affiche le message d �erreur ***/  
       exit(-1);    /*** On sort en indiquant un code erreur ***/
    } 
    printf("Socket creee avec succes!  (%d)\n",sockfd); 
   
   /* Cr�ation de l'adresse externe du socket serveur */
   longueurAdresse = sizeof(struct   sockaddr_in); 
   memset(&ServAddr, 0x00, longueurAdresse); 
   ServAddr.sin_family = AF_INET; 
   ServAddr.sin_addr.s_addr =  htonl(INADDR_ANY);    
   ServAddr.sin_port = htons(ServPort);    

   /* Attachement de socket */
   /*** bind - Fournir un nom � une socket ***/
   /*** Description: 
		bind() affecte l'adresse sp�cifi�e dans ServAddr � la socket r�f�renc�e par le descripteur de fichier sockfd. 
		longueurAdresse indique la taille, en octets, de la structure d'adresse point�e par ServAddr . 
		Une adresse externe est attribu�e gr�ce � la fonction bind**/

   /*** L�appel renvoie 0 s�il  r�ussit, ou -1 s�il  �choue ***/
   if((bind(sockfd,(struct  sockaddr *)&ServAddr, longueurAdresse)) <  0) 
   { 
       perror("bind"); 
       exit(-2); 
   } 
   printf("Socket attachee avec succes!\n"); 

   /*** Obtenir la longueur en octets de la structure sockaddr_in ***/
   longueurAdresse = sizeof(ClientAddr); 
   /*** Initialise � 0 la structure sockaddr_in c�est l�appel recvfrom qui remplira cette structure ***/
   memset(&ClientAddr, 0x00, longueurAdresse); 
   /*** Initialise � 0 le message ***/
   memset(Buffer, 0x00, MAX*sizeof(char));  
   /* R�ception d'un message du client */
   /***  recvfrom - Recevoir un message sur une socket ***/
   /***  L�appel syst�me recvfrom() est utilis� pour recevoir des messages **/
   countr = recvfrom(sockfd, Buffer,  sizeof(Buffer), 0, (struct  sockaddr *)&ClientAddr, &longueurAdresse); 
   /*** Cet appel syst�me renvoie le nombre d�octets re�us s'il r�ussit, ou -1 s'il 
        �choue ***/ 
   /*** C'est l'appel recvfrom() qui remplit la structure sockaddr_in avec les informations du point de com- 
         munication du client (adresse IP et num�ro de port) ***/
    switch(countr) 
   { 
       case  -1  : /***  une  erreur  ***/ 
          perror("recvfrom"); 
          close(sockfd); 
          exit(-3); 
       case  0 : 
          fprintf(stderr, "Aucune donn�e n�a �t� re�ue!\n\n"); 
          close(sockfd); 
          return   0; 
       default :  /***  r�ception   de  n octets   ***/
             printf("Message %s recu avec succes (%d  octets)\n\n", Buffer, countr); 
   } 
   
   /* Fermeture de socket */
   close(sockfd); 

   return   0; 
} 
