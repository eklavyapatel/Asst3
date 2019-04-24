#include "wtf.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    
    const char* IPaddress;
    int portNum;
    //tries to open configure file
    int fd = open("configure", O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    //if found, that means that configure has already been run, good
    if (fd < 0) {
        if (errno == EEXIST) {
            printf("file found! \n");
            close(fd);
            int fd = open("configure", O_RDONLY );
            if(fd <0){
                printf("Error: Could not open file. Please try again.");
            }
            
            char * temp = (char *)malloc(sizeof(char) * 50);
            int total_length = read(fd, temp, 50);
            char* input = (char *)malloc(sizeof(char) * (total_length + 1));
            strcpy(input, temp);
            free(temp);
            char* delim = "\n";
            IPaddress = strtok(input, delim);
            portNum = atoi(strtok(NULL, delim));
            
            printf ("this is the IP address: %s \n", IPaddress);
            printf ("this is the Port Number: %d \n", portNum);
            close(fd);
        }
    //otherwise check if user has inputed valid requirements for configure
    } else {
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
        //write the IP Address to file
        write(fd_configure, argv[2], strlen(argv[2]));
        //write the port num to file on new line
        write(fd_configure, "\n", 1);
        write(fd_configure, argv[3], strlen(argv[3]));
        write(fd_configure, "\n", 1);
        close(fd_configure);
        //successful configuration
        printf("Successful configuration. \n");
        return EXIT_SUCCESS;
    }

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
    int sockfd, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    //signal(SIGINT, ctrlC_shutdown);
    
    char buffer[256];
 
    //connect to server for further operations
    //portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("Error: Could not open socket. \n");
    }

    server = gethostbyname(IPaddress);
    if (server == NULL) {
        fprintf(stderr,"Error: No such host. \n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portNum);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("Error: Could not connect. \n");
    }
    printf("Please enter the message: ");
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
