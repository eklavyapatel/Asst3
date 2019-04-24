#include "wtf.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    
    struct stat search;
    //checks for configure file in the directory
    int exist  = stat ("./.configure", &search);
    //if file does not exist, this is the first call, so do configure
    if(exist == 0){
        //check for valid number of inputs
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        //check if the first command is configure
        if((strcmp(argv[1], "configure")) != 0){
            printf("Error: First command must be configure. \n");
            return EXIT_FAILURE;
        }
        //create the configure file
        int fd_configure = open("configure", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_configure < 0) {
            printf("Error: Could not create configure file. Please try again.\n");
            return EXIT_FAILURE;
        }
        //write the length "\t" IP Address to file
        write(fd_configure, argv[2], strlen(argv[2]));
        //write the IP Address to file on new line
        write(fd_configure, "\n", 1);
        write(fd_configure, argv[3], strlen(argv[3]));
        close(fd_configure);
        //successful configuration
        printf("Successful configuration. \n");
        return EXIT_SUCCESS;
    }
    if(exist != 0){
        printf("file found! \n");
    }
    //if the file exists, open it and read the IP Address and Port Number
    int fd_configure = open("configure", O_RDONLY , 0644);
    char* IPaddress;
    char* portNum;
    //read 50 bytes abritrarily for the IP Address
    read(fd_configure, IPaddress, 50);
    //read 6 bytes for Port Number because it can be 5 digits at most with 1 space for the null terminator
    read(fd_configure, portNum, 6);
    close(fd_configure);
    
    int sockfd, n;
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //signal(SIGINT, ctrlC_shutdown);
    
    char buffer[256];

    //if firstRun is 0, this will print out to check if the extern values are working
    printf("This is the IP Address: %s \n", IPaddress);
    printf("This is the Port Number: %s \n", portNum);
    //perform add and remove first
    if((strcmp(argv[1], "add")) == 0){
        printf("good");
        return EXIT_SUCCESS;
    }
    //perfrom removal of files
    if((strcmp(argv[1], "remove")) == 0){
        printf("good");
        return EXIT_SUCCESS;
    }
    //connect to server for further operations
    //portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("Error: Could not opening socket. \n");
    }
    server = gethostbyname(IPaddress);
    if (server == NULL) {
        fprintf(stderr,"Error: No such host. \n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(atoi(portNum));
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("Error: Could not connect. \n");
    }
    //printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
        error("Error: Could not write to socket. \n");
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){
        error("Error: Could not read from socket. \n");
    }
    printf("%s\n",buffer);
    return 0;
}
