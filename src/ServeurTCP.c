#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <unistd.h>       
#include <netinet/in.h> 
#include <stdio.h>
#include <mysql/mysql.h>
//#define SERVER_TCP_PORT 8082

double montant() {
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Initialiser la connexion MySQL
    conn = mysql_init(NULL);

    // Établir une connexion à la base de données
    if (mysql_real_connect(conn, "localhost", "root", "", "socketdb", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Erreur de connexion à la base de données : %s\n", mysql_error(conn));
        return 1;
    }

    // Exécuter une requête SQL
    if (mysql_query(conn, "SELECT * FROM facture")) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        return 1;
    }

    // Récupérer le résultat de la requête
    result = mysql_store_result(conn);
    double count=0;

    // Afficher les données
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("Colonne 1 : %s, Colonne 2 : %s\n,Colonne 3 : %s\n", row[0], row[1],row[2]);
        count+=atof(row[2]);
        printf("count =%lf\n",count);
         
    }

    // Libérer les ressources
    mysql_free_result(result);
    mysql_close(conn);
    return count;
}
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
    //ServAddr.sin_port = htons(SERVER_TCP_PORT); 
    ServAddr.sin_port = htons(5000); 
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
        printf("le message reçu est %c\n",received_buffer);
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
                
                double montant_a_envoyer=montant();             
                send(dialogue_socket,&montant_a_envoyer,sizeof(double),0);
               printf("Message envoyé \n");
        }     
    }
    close(dialogue_socket);  
    close(sockfd); 
    return 0; 
} 




