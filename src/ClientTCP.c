#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_LENGTH 1024
int main(int argc, char *argv[]) {
    int client_socket; 
    int8_t input;
    struct sockaddr_in server_address = {0}; 
    client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket < 0){
        perror("Failed to create client socket");
        return -1;
    }
    printf("Created client socket\n");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    while (connect(client_socket,(struct sockaddr *)&server_address,sizeof(server_address)) == -1) sleep(1);
    printf("Connected to http server\n");
    printf("==============================\n");
    while (1){
        printf("1) Consult sales invoices for parapharmaceutical products\n");
        printf("2) Retrieve sales invoices for transport vehicles\n");
        printf("3) Receive the overall revenue\n");
        printf("0) Close Application\n");
        printf("Select an action: ");        
        scanf("%c",&input);
        for(int c;(c = getchar()) != '\n' && c != EOF;)input = -1;//to discard other input 
        input -= '0';//because we got the input as char 
        if(input < 0 || input > 3)
            printf("non valid character\n");
        else if(input == 0){
            printf("shutting down client");
            shutdown(client_socket,SHUT_RDWR);
            close(client_socket);
            return 0;
        }
        else{
            send(client_socket,&input,sizeof(int8_t),0);

            char received_buffer[MAX_BUFFER_LENGTH];
            memset(received_buffer,0x00,MAX_BUFFER_LENGTH);
            recv(client_socket,received_buffer,MAX_BUFFER_LENGTH,0);
            printf("receive buffer from proxy: \n%s\n",received_buffer);
        }
        printf("================================\n");
    }
}
