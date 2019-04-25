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
            //i closed it and opened it again because for some reason i was getting an error
            int fd = open("configure", O_RDONLY );
            if(fd <0){
                printf("Error: Could not open file. Please try again.");
            }
            //copy all
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
    printf("Connection Successful!");
    //Upon successful connection
    
    //::::::::::::: C H E C K :: O U T :::::::::
    if((strcmp(argv[1], "checkout")) == 0){
        //./WTF checkout <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: U P D A T E :::::::::
    if((strcmp(argv[1], "update")) == 0){
        //./WTF update <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: U P G R A D E :::::::::
    if((strcmp(argv[1], "upgrade")) == 0){
        //./WTF upgrade <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: C O M M I T :::::::::
    if((strcmp(argv[1], "commit")) == 0){
        //./WTF commit <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: P U S H :::::::::
    if((strcmp(argv[1], "push")) == 0){
        //./WTF push <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: C R E A T E :::::::::
    if((strcmp(argv[1], "create")) == 0){
        //./WTF create <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: D E S T R O Y :::::::::
    if((strcmp(argv[1], "destroy")) == 0){
        //./WTF destroy <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: C U R R E N T :: V E R S I O N :::::::::
    if((strcmp(argv[1], "currentversion")) == 0){
        //./WTF currentversion <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: H I S T O R Y :::::::::
    if((strcmp(argv[1], "history")) == 0){
        //./WTF history <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    //::::::::::::: R O L L B A C K :::::::::
    if((strcmp(argv[1], "rollback")) == 0){
        //./WTF rollback <project name> <version>
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        
        
        return EXIT_SUCCESS;
    }
    
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
