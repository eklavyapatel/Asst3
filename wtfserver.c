#include "wtf.h"

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
int sock_des = 0;

//handles the client on a particular thread. must command calls go in here
void *clientHandler(void* client_socket){
    int socket_num = *(int*) client_socket;

    char client_message[256];
    memset(client_message, '\0', sizeof(client_message));

    while(1){
        //lets reset the string storage client_message before starting
        memset(client_message, '\0', sizeof(client_message));

        //this will read from socket and store message in client_message char array
        int read_return = read(socket_num, client_message, 255);
        if(read_return  == -1){
            //we are going to need to send back read error, please try again
            write(socket_num, "We had trouble reading that. Please try again.", 52);
        }
        
        const char s[2] = ":";
        
        char* command = strtok(client_message, s);

        read_return = read(socket_num, client_message, 255);//this will read from socket and store message in client_message char array
        if(read_return  == -1)
        {
            //we are going to need to send back read error, please try again
            write(socket_num, "We had trouble reading that. Please try again.", 52);

        }
        
        if(strcmp(command, "checkout")){
            //server-side stuff fore commit goes here
        }else if(strcmp(command, "update")){

        }else if(strcmp(command, "uprade")){

        }else if(strcmp(command, "commit")){

        }else if(strcmp(command, "push")){

        }else if(strcmp(command, "create")){

            //recieves the protocol for create, decifer and execute.
            int projectNameLength;
            char* projectName;
            
            //now create the directory in repo
            printf ("%d\n",projectNameLength);
            printf ("%s\n",projectName);
            
            
            
            
            
            
            return EXIT_SUCCESS;
            
        }else if(strcmp(command, "destroy")){
            //Step 1: Lock the repo
            //Step 2: Expire and pending commits
            //Step 3: Delete all files and subdirectories in the project directory
            //Step 4: Send success message
        }else if(strcmp(command, "currentversion")){
            //Step 1: 
        }else if(strcmp(command, "history")){

        }else if(strcmp(command, "rollback")){

        }else{
            //if not valid command
        }
        close(socket_num);
    }
        return NULL;
}

//main function madafaka
int main(int argc, char *argv[]) {

    
    //signal(SIGINT, ctrlC_shutdown);
    
    int portno;
    socklen_t clilen;

    //signal(SIGINT, ctrlC_shutdown);

    char buffer[256];
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    int n;
    if (argc != 2) {
        printf("Error:Invalid number of arguments. \n");
        return EXIT_FAILURE;
    }

    //ok create repo directory
   //int check = mkdir(".server_repo", );

    //int check = mkdir(".server_repo");

    sock_des = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_des < 0)
        printf("Error: Could not create socket. \n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sock_des, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        printf("Error: Could not perform binding. \n");
    }
    listen(sock_des,5);

    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);

    //multithreading tings
    int client_socket;
    while(1){
        client_socket = accept(sock_des, (struct sockaddr*) &cli_addr, &clilen);
        if(client_socket > 0){
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
    /*newsock_des = accept(sock_des, (struct sockaddr *) &cli_addr, &clilen);
    if (newsock_des < 0){
        printf("Error: Could not perform accept. \n");
    }
    bzero(buffer,256);
    n = read(newsock_des,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    //reads from client here
    char* command = strtok(buffer, ":");
    if (strcmp(command, "create")==0){
        //operate on create

    }


    printf("Here is the message: %s\n",buffer);



    n = write(newsock_des,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");*/
    return EXIT_SUCCESS;
}
