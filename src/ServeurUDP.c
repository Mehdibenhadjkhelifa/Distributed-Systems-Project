#include <stdio.h>      // pour printf() et fprintf()
#include <stdbool.h>
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <stdlib.h>     // pour atoi(), exit() 
#include <string.h>     // pour memset(), strcpy()  
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in

#define SERVER_UDP_PORT 8081
bool get_file_content(const char* file_path,char* buffer,size_t* buffer_size);
int  main(int argc, char * argv[]) 
{ 
    int  sockfd;                   // descripteur de socket 
    struct sockaddr_in server_address;   // @ du serveur
    struct sockaddr_in client_address; // @ du client
    unsigned int longueurAdresse = sizeof(struct   sockaddr_in); 
    char  received_buffer;// received_buffer de réception
    size_t buffer_size = 1;
    char* buffer_to_send = (char*)calloc(buffer_size,sizeof(char));
    sockfd = socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd < 0)  /*** échec ? ***/
    { 
        perror("Probleme de creation de socket\n ");  /***  Affiche le message d ’erreur ***/  
        exit(-1);    /*** On sort en indiquant un code erreur ***/
    } 
    printf("Socket creee avec succes!  (%d)\n",sockfd); 
    memset(&server_address, 0x00, longueurAdresse); 
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr =  htonl(INADDR_ANY);    
    server_address.sin_port = htons(SERVER_UDP_PORT);    
    if((bind(sockfd,(struct  sockaddr *)&server_address, longueurAdresse)) <  0) 
    { 
        perror("bind"); 
        exit(-2); 
    } 
    printf("Socket attachee avec succes!\n"); 
    while(1){
        memset(&client_address, 0x00, longueurAdresse); 
        ssize_t countr = recvfrom(sockfd, &received_buffer,  sizeof(received_buffer), 0, (struct  sockaddr *)&client_address, &longueurAdresse); 
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
                memset(buffer_to_send,0x00,buffer_size);
//                printf("Message %d recu avec succes (%zd  octets)\n\n", received_buffer, countr); 
                if(!get_file_content("FacturePara.txt",buffer_to_send,&buffer_size))
                    exit(-2);
                sendto(sockfd,buffer_to_send,buffer_size,0,(struct sockaddr*)&client_address,longueurAdresse);
        } 
    }
    free(buffer_to_send);
    /* Fermeture de socket */
    close(sockfd); 

   return   0; 
} 
bool get_file_content(const char* file_path,char* buffer,size_t* buffer_size){
    FILE *file;
    size_t file_size;

    // Open the file in binary mode
    file = fopen(file_path,"r");

    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    file_size = (size_t)ftell(file);
    rewind(file);
    printf("the file_size is %zu\n",file_size);

    if(file_size + 1 > *buffer_size){
        void* temp =realloc(buffer,file_size + 1); // +1 for the null terminator
        if (buffer == NULL || temp == NULL) {
            perror("Error reallocating memory");
            fclose(file);
            return false;
        }
        *buffer_size =(size_t) file_size + 1;
    }

    // Read the file content into the buffer
    fread(buffer,sizeof(char), file_size, file);

    // Null-terminate the buffer
    buffer[file_size] = '\0';
    // Close the file
    fclose(file);
    return true;
}
