#include "wtf.h"

//will this stay changed on the next run if I set it to 0 elsewhere
int firstRun = 1;

char* IPaddress;
int portNum;

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //signal(SIGINT, ctrlC_shutdown);
    
    char buffer[256];
    
    if(firstRun){
        //check if correct number of arguments
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        //check if the first command is configure
        if((strcmp(argv[1], "configure")) != 0){
            printf("Error: First command must be configure. \n");
            return EXIT_FAILURE;
        }
        // indexes 2 and 3 must have IP Address and Port Number, respectively.
        // store them in environment variables.
        IPaddress = argv[2];
        portNum = atoi(argv[3]);
        //successful configuration
        printf("Successful configuration. \n");
        firstRun = 0;
        return EXIT_SUCCESS;
    }
    
    printf("This is the IP Address: %s \n", IPaddress);
    printf("This is the Port Number: %d \n", portNum);
    //perform add and remove first
    
    //connect to server for further operations
    //portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(IPaddress);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portNum);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
