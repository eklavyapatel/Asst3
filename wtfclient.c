#include "wtf.h"
int sockfd = -1;

int main(int argc, char *argv[]){

    const char* IPaddress;
    int portNum;
    //tries to open configure file
    int fd = open("./.configure", O_WRONLY , S_IRUSR | S_IWUSR);
    //if found, that means that configure has already been run, good
    if (fd > 0) {
            printf("file found! \n");
            close(fd);
            //i closed it and opened it again because for some reason i was getting an error
            int fd = open("./.configure", O_RDONLY );
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

            free(input);

            printf ("this is the IP address: %s \n", IPaddress);
            printf ("this is the Port Number: %d \n", portNum);
            close(fd);
    //otherwise check if user has inputed valid requirements for configure
    } else {
        //::::::::::::: C O N F I G U R E :::::::::
        //check if the first command is configure
        if((strcmp(argv[1], "configure")) != 0){
            printf("Error: First command must be configure. \n");
            return EXIT_FAILURE;
        }
        //./WTF configure <IP> <port>
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }

        //create the configure file
        int fd_configure = open("./.configure", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

    //::::::::::::: A D D :::::::::
    if((strcmp(argv[1], "add")) == 0){
        //./WTF add <project name> <filename>
        printf("good. \n");
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    //::::::::::::: R E M O V E :::::::::
    if((strcmp(argv[1], "remove")) == 0){
        //./WTF remove <project name> <filename>
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        printf("good. \n");
        return EXIT_SUCCESS;
    }
    printf("Welcome! Attempting to connect to host \n");
    int n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //signal(SIGINT, ctrlC_shutdown);

    printf("%s\n", IPaddress);
    server = gethostbyname(IPaddress);
    printf("%s\n", IPaddress);
    if (server == NULL) {
        printf("Error: No such host. \n");
        return EXIT_FAILURE;
    }
    
    //connect to server for further operations
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("Error: Could not open socket. \n");
    }

    memset(&serv_addr, '\0', sizeof(serv_addr));
    
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portNum);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        printf("Error: Could not connect. \n");
    }
    
    socklen_t size = sizeof(serv_addr);
    
    //wait for 3 seconds
    while(connect(sockfd, (struct sockaddr *)&serv_addr,size)<0)
    {
        printf("Error: Could not connect. Trying Again In 3 Seconds\n");
        sleep(3);
    }
    
    printf("Connection Successful!\n");
    //Upon successful connection

    char buffer[256];
    
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

        char* projectName = argv[2];
        int nameLength = strlen(argv[2]);

        //create a path to client side Manifest
        char pathToMan [256];
        strcpy(pathToMan, "./");
        strcat(pathToMan, projectName);
        strcat(pathToMan, "/.Manifest");
        printf("Path to Manifest: %s \n", pathToMan);


        bzero(buffer, 256);
        //protocol for update
        sprintf(buffer, "update:%d:%s", nameLength, projectName);
        //write to socket
        int i = write(sockfd, buffer, strlen(buffer));
        if(i < 0){
          printf("Error: Could not write into socket. \n");
        }
        //reset buffer to read manifest or error from socket
        bzero(buffer, 255);
        //read response
        i = read(sockfd,buffer,255);
        if(i < 0){
          printf("Error: Could not read from socket. \n");
        } else {
          printf("Message from server recieved. \n");
        }
        //error handling if say project doesn't exists
        int fd2;
        if(strcmp(buffer, "Project .Manifest does not exist on server side. \n") == 0){
          return EXIT_FAILURE;
        }
          //server sends protocol of Manifest
          //have to decode that manifest hash to get its content
          //have to read and compare with the client side manifest
          int sfd = open(sockfd,O_RDONLY);
          bzero(buffer,256);
          read(sockfd,buffer,255);
          //now buffer has the protocol Message - tokenize and decode

          char * token = strtok(buffer,":");
          int counter = 1;
          int length;
          char dirName[256];
          while(token != NULL){
            if(counter == 1 || counter = 3){
              length = atoi(token);
            } else if(counter = 2){
              sprintf(dirName, token);
            } else if(counter = 4){
              //decode this
            }

          }

        char * clientman = (char*)malloc(INT_MAX);
        char * serverman = (char*)malloc(INT_MAX);
        //lets open client side manifest over here
        int fd = open(pathToMan, O_RDONLY);
        if(fd < 0){
          printf("Error: Could not open client side Manifest. \n");
        }
        //reading client manual
        read(fd,clientman,INT_MAX);
        //idk if i will have to read server manifest since i will be decoding the hash
        read();

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
        char* projectName = argv[2];
        int projectLength = strlen(projectName);

        bzero(buffer,256);
        //inputs protocol onto buffer
        //protocol is create:<length of project name>:<project name>
        sprintf(buffer, "create:%d:%s", projectLength, projectName);
        //writes on to socket descriptor
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0){
            printf("Error: Could not write to socket. \n");
        }
        bzero(buffer,256);
        //reads response from server onto buffer
        n = read(sockfd,buffer,255);
        if (n < 0){
            printf("Error: Could not read from socket. \n");
        }else{
            printf("Message from server recieved. \n");
        }
        printf("%s\n",buffer);
        //Error handling an existing project
        if (strcmp(buffer,"Project name already exists") == 0){
            printf("Error: Project already exists. Please try again.\n");
            return EXIT_FAILURE;
        }else{
            //otherwise server sends back the protocol for the manifest file.
            //create local version of the project and add this manafest file too it
            3
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
        char* projectName = argv[2];
        int nameLength = strlen(argv[2]);

        bzero(buffer, 256);
        //send protocol to the server
        sprintf(buffer,"destroy:%d:%s", nameLength, projectName);
        int n = write(sockfd, buffer, strlen(buffer));
        if(n < 0){
          error("Error: Could not write to socket. \n");
        } else {
          printf("Message sent.\n");
        }

        //read from socket - this confirms whether the task was completed or failed
        n = read(sockfd, buffer, 255);
        if(n < 0){
          error("Error: Could not read from socket. \n");
        } else {
          printf("Message from server recieved. \n");
        }
        //check which type of error this is
        if(strcmp(buffer, "Project name doesn't exist") == 0){
          printf("Error: Project doesn't exist - can't delete %s.\n", projectName);
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

        char* projectName = argv[2];
        int nameLength = strlen(argv[2]);

        bzero(buffer, 256);
        //make the protocol in buffer
        sprintf(buffer, "currentversion:%d:%s",nameLength,projectName);
        //write protocol to server
        int n = write(sockfd, buffer, strlen(buffer));
        if(n < 0) {
          error("Error: Could not write to socket. \n");
        } else {
          printf("Message sent.\n");
        }

        //read message from socket
        n = read(sockfd, buffer, 255);
        if(n < 0){
          error("Error: Could not read from socket. \n");
        } else {
          printf("Message from server recieved. \n");
        }
        //check for errors
        if(strcmp(buffer, "Project does not exist.") == 0){
          printf("Error: Project doesn't exist can not get current version.\n");
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
        printf("Error: Could not write to socket. \n");
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){
        printf("Error: Could not read from socket. \n");
    }
    printf("%s\n",buffer);
    return 0;
}
