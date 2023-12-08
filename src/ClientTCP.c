#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
int main(int argc, char *argv[]) {
    int sclient; 
    int8_t input;
    struct sockaddr_in saddr = {0}; 
    sclient = socket(AF_INET,SOCK_STREAM,0);
    if(sclient < 0){
        perror("Failed to create client socket");
        return -1;
    }
    printf("Created client socket\n");
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    while (connect(sclient,(struct sockaddr *)&saddr,sizeof(saddr)) == -1) sleep(1);
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
            shutdown(sclient,SHUT_RDWR);
            close(sclient);
            return 0;
        }
        else{
            send(sclient,&input,sizeof(int8_t),0);
        }
        printf("================================\n");
    }
}
