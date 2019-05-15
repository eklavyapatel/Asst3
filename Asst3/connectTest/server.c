#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

typedef struct threadsLL
{
    int sockfd;
    pthread_t name;
    struct threadsLL* next;
} threadsLL;

//set up the LL for threadsLL
threadsLL* front = NULL;
threadsLL* traverser = NULL;


//make the socket descriptor a global variable
int sockfd = 0;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void *clientHandler(void* client_socket){
    int socket_num = *(int*) client_socket;
    
    char client_message[256];
    memset(client_message, '\0', sizeof(client_message));
    
    int read_return = 0;
    while(1)
    {
        //lets reset the string storage client_message before starting
        memset(client_message, '\0', sizeof(client_message));
        
        read_return = read(socket_num, client_message, 255);//this will read from socket and store message in client_message char array
        if(read_return  == -1)
        {
            //we are going to need to send back read error, please try again
            write(socket_num, "We had trouble reading that. Please try again.", 52);
            
        }
        
        //now we will parse the text to see what we have been requested to do
        if(strncmp(client_message, "checkout ", 9) == 0){
        }else if(strncmp(client_message, "update ", 6) == 0){
        }else if(strncmp(client_message, "upgrade ", 6) == 0){
        }else if(strncmp(client_message, "commit ", 6) == 0){
        }else if(strncmp(client_message, "push ", 6) == 0){
        }else if(strncmp(client_message, "create ", 6) == 0){
        }else if(strncmp(client_message, "destroy ", 6) == 0){
        }else if(strncmp(client_message, "currentversion ", 6) == 0){
        }else if(strncmp(client_message, "histroy ", 6) == 0){
        }else if(strncmp(client_message, "rollback ", 6) == 0){
        }
    }
}

int main(int argc, char *argv[]){
    int socketfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    /*if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }*/
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }
    
    listen(sockfd,5);
    
    clilen = sizeof(cli_addr);
    
    //multithreading happens here
    int client_socket;
    while(1){
        client_socket = accept(socketfd, (struct sockaddr*) &cli_addr, &clilen);
        if (client_socket < 0){
            error("ERROR on accept");
        }
        //now we need to check if we have actually recieved a proper client
        if(client_socket > 0)
        {
            printf("We have successfully connected to the Client\n");
            
            threadsLL* newNode = (threadsLL*) malloc(sizeof(threadsLL));
            newNode->next = NULL;
            newNode->sockfd = client_socket;
            if(front == NULL){
                //printf("Front was pointing to NULL, now points to the first node\n");
                front = newNode;
            }else {
                //printf("We added a new node to the LL\n");
                newNode->next = front;
                front = newNode;
            }
            
            pthread_create(&newNode->name, NULL, clientHandler, &client_socket);
        }
    }
    
    /*newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");*/
    return 0;
}
