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
#define SERVER_TCP_PORT 8082
#define VEHICULE_CODE 2 
void* montant(int code) {
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;
    double* total_cost;
    char* str_invoice;
    size_t str_size = strlen("id      Name      Amount\n");
    // Initialiser la connexion MySQL
    conn = mysql_init(NULL);

    // Établir une connexion à la base de données
    if (mysql_real_connect(conn, "localhost", "root", "root", "socketdb", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Erreur de connexion à la base de données : %s\n", mysql_error(conn));
        return NULL;
    }

    // Exécuter une requête SQL
    if (mysql_query(conn, "SELECT * FROM votre_table")) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        return NULL;
    }

    // Récupérer le résultat de la requête
    result = mysql_store_result(conn);
    if(code == VEHICULE_CODE){
        str_invoice = (char*)calloc(str_size + 1,sizeof(char));//+1 for null termination
        strncpy(str_invoice,"id Name Amount\n",str_size);
    }
    else{
        total_cost=(double*)calloc(1,sizeof(double));
    }
    
    // Afficher les données
    while ((row = mysql_fetch_row(result)) != NULL) {
        if(code == VEHICULE_CODE){
            char result_str[50];
            memset(result_str,0x00,50);
            sprintf(result_str,"%s    %s     %s\n",row[0],row[1],row[2]);
            //printf("%s\n",result_str);
            printf("str length is : %lu\n",strlen(result_str));
            printf("Str size before change : %lu\n",str_size);
            str_size += strlen(result_str);
            printf("Str size after change : %lu\n",str_size);
            printf("the result before now is : %s",str_invoice);
            str_invoice =(char*)realloc(str_invoice,(str_size * sizeof(char)) + 1);
            strcat(str_invoice,result_str);
            //get the last character of the string to append to it with null termination character
            printf("the result after now is : %s",str_invoice);
        } 
        else{
            *total_cost+=atof(row[2]);
            printf("count =%lf\n",*total_cost);
        }
        printf("Colonne 1 : %s, Colonne 2 : %s,Colonne 3 : %s\n", row[0], row[1],row[2]);

    }

    // Libérer les ressources
    mysql_free_result(result);
    mysql_close(conn);
    return code == VEHICULE_CODE ? (void*)str_invoice : (void*)total_cost;
}
int  main(int argc, char* argv[]) 
{
    int  sockfd;                      
    struct sockaddr_in ServAddr;     
    struct sockaddr_in ClientAddr;    
    socklen_t   longueurAdresse; 
    char  received_buffer = -1;               
    char* buffer_to_send ;
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
    ServAddr.sin_port = htons(SERVER_TCP_PORT); 
    //ServAddr.sin_port = htons(5000); 
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
               if(received_buffer == VEHICULE_CODE){
                   buffer_to_send = (char*)montant(received_buffer);
                   send(dialogue_socket,buffer_to_send,strlen(buffer_to_send),0);
                   free(buffer_to_send);
               }
               else{
                double* montant_a_envoyer=(double*)montant(received_buffer);             
                send(dialogue_socket,montant_a_envoyer,sizeof(double),0);
                free(montant_a_envoyer);
               }
               printf("Message envoyé \n");
        }     
    }
    close(dialogue_socket);  
    close(sockfd); 
    return 0; 
} 




