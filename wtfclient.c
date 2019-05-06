#include "wtf.h"

//Function to Hash file contents with SHA256
char* calcHash(char* filepath) {
    printf("in hash function\n");
    FILE *content = fopen(filepath,"rb");
    if (content == NULL){
        printf("Error: Couldn't open file.\n");
        return NULL;
    }

    fseek(content, 0, SEEK_END);
    long fsize = ftell(content);
    fseek(content, 0, SEEK_SET);
    
    char *s = malloc(fsize + 1);
    fread(s, 1, fsize, content);
    fclose(content);
    printf("file work done\n");
    
    SHA256_CTX sha256;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char* output = (char*)malloc(64*sizeof(char));
    SHA256_Init(&sha256);
    int i = 0;
    int bytes = 0;
    
    while(bytes < fsize) {
        SHA256_Update(&sha256, s, bytes);
        bytes++;
    }
    SHA256_Final(hash, &sha256);
    
    while (i < SHA256_DIGEST_LENGTH) {
        sprintf(output + i*2, "%02x", hash[i]);
        i++;
    }
    return output;
}

// checks if the file exists
int exists(char *dir,char* project) {
    char* projectpath = (char*)malloc((9+strlen(project))*sizeof(char));
    projectpath = strcpy(projectpath, "./");
    projectpath = strcat(projectpath, project);
    
    DIR *folder = opendir(projectpath);
    if(folder) {
        close(dir);
        printf("folder %s exists\n", projectpath);
        return 1;
    }
    printf("folder %s does not exist\n", projectpath);
    return 0;
}
return 0;
}


//converts server path to client path
char* dirpath(char* project, char* file) {
    if(strcmp(file, "done") == 0) {
        return file;
    }
    int length = strlen(file) - strlen(strrchr(file, '/'));
    int i;
    char* projectpath = (char*)malloc(length*sizeof(char));
    for(i = 0; i < length; i++) {
        projectpath[i] = file[i];
    }
    projectpath[length] = '\0';
    return projectpath;
}

int add (char* projectName, char* filePath){
    //1. Check if Project exists
    struct dirent* traverse;
    DIR* project = opendir(projectName);
    if (project){
        //Project Exists
        //2.traverse Directory to check for file duplicates
        //first extract filename from path
        char *fileName = (char *) malloc(50);
        //printf("%s\n",filePath);
        strcpy(fileName, filePath);

        char *ssc;
        int l = 0;
        ssc = strstr(fileName, "/");
        do{
            l = strlen(ssc) + 1;
            fileName = &fileName[strlen(fileName)-l+2];
            ssc = strstr(fileName, "/");
        }while(ssc);
        //printf("%s\n", fileName);
        while ((traverse = readdir(project)) != NULL){
            if((strcmp(traverse->d_name,fileName)) ==0){
                printf("Error: %s already exists in Repository. Please try another name. \n", traverse->d_name);
                return 0;
            }
            //printf("%s\n", traverse->d_name);
        }
        closedir(project);
        //free(fileName);
        //3.Hash the contents of the file
        
        char *hashCode = (char *) malloc(100);
        hashCode = calcHash(filePath);
        //strcpy(hashCode,calcHash(filePath));
        //printf("%s\n", hashCode);

        //4.look for and open .Manifest file and add to it
        char manifestPath[50];
        sprintf(manifestPath, "./%s/Manifest",projectName);
        FILE* manifest = fopen(manifestPath, "ab");
        if(manifest == NULL){
            printf("Error: Manifest file not initiated. Invalid Project.\n");
            return 0;
        }
        char toAdd [100];
        sprintf(toAdd,"1\t%s\t%s\n",filePath,hashCode);
        //printf("%s\n",toAdd);
        //adds line to end of file
        fputs(toAdd, manifest);
        fclose(manifest);
        return 1;
    }else if(ENOENT == errno){
        //Project doesn't exist
        printf("Error: %s does not exist. Please try again.\n",projectName);
        return 0;
    }else{
        printf("other reason\n");
    }
}

void checkout (char* projectName, int sockfd){
    if(exists(".", projectName) == 1) {
        printf("Error: Project already exists on local branch.\n");
        //return;
    }
    
    char protocol[256];
    bzero(buffer,256);
    //inputs protocol onto buffer
    //protocol is checkout:<project name>
    sprintf(buffer, "checkout:%s", projectName);
    //writes on to socket descriptor
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
        printf("Error: Could not write to socket. \n");
    }
    char buffer[1024];
    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    if(strcmp(buffer, "project not found") == 0) {
        printf("Message from server: %s\n", buffer);
        bzero(buffer, 1024);
        return;
    }else {
        mkdir(project, 0777);
    }
    int n;

    n = write(sockfd,"next",5);
    if (n < 0){
        printf("Error: Could not write to socket. \n");
    }
    while(1) {
        //send(sock, "next", 5, 0);
        bzero(buffer, 1024);
        n = read(sockfd, buffer, 1024);
        if(strcmp(buffer, "done") == 0) {
            break;
        }
        char* filepath = (char*)malloc(256*sizeof(char));
        int i, j;
        for(i = 0; i < strlen(buffer); i++) {
            j = i+9;
            filepath[i] = buffer[j];
        }
    }
    char* path = dirpath(project, filepath);
    printf("path: %s\n", path);
    mkdir(path, 0777);
    printf("file %s\n", filepath);
    int fd = open(filepath, O_RDWR | O_TRUNC | O_CREAT, 0644);
    send(sock, "next", 5, 0);
    bzero(buffer, 1024);
    read(sock, buffer, 1024); //contents
    //printf("contents %s\n", buffer);
    if(strcmp(buffer, "empty\a") != 0) {
        write(fd, buffer, strlen(buffer));
    }
    send(sock, "next", 5, 0);
}
bzero(buffer, 1024);
}

int main(int argc, char *argv[]){

    char IPaddress[30];
    char portNum[6];

    FILE* fp_configure = fopen("./.configure","r");

    //if opening for read not possible, then it doesnt exist, so configure
    if(fp_configure == NULL){
        /******************** C O N F I G U R E  *****************************/
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
        //create the file
        //"ab" - open or create file for writing at end-of-file.
        FILE* fp;
        fp = fopen("./.configure","ab");
        //int argPortNum = atoi(argv[3]);
        //fprintf(fp,"%s %d",argv[2],argPortNum);
        fputs(argv[2],fp);
        fputs(" ",fp);
        fputs(argv[3],fp);
        fclose(fp);

        //successful configuration
        printf("Successful configuration. \n");
        return EXIT_SUCCESS;
    }
    printf("file found! \n");

    fscanf(fp_configure,"%s %s",IPaddress,portNum);

    fclose(fp_configure);

    printf ("this is the IP address: %s \n", IPaddress);
    printf ("this is the Port Number: %s \n", portNum);

    /******************** A D D *****************************/
    if((strcmp(argv[1], "add")) == 0){
        //./WTF add <project name> <filename>
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        char* projectName = argv[2];
        char* filePath = argv[3];
        if((add(projectName, filePath)) == NULL){
            printf("Error: Failure in adding file to Repository\n");
            return EXIT_FAILURE;
        }else{
            printf("File successfully added to Repository\n");
            return EXIT_SUCCESS;
        }
    }
    /******************** R E M O V E *****************************/
    if((strcmp(argv[1], "remove")) == 0){
        //./WTF remove <project name> <filename>
        if(argc != 4) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        printf("good. \n");
        return EXIT_SUCCESS;
    }
    /**************** C O N N E C T  S E R V E R  *************************/
    printf("Welcome! Attempting to connect to server. \n");
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    //connect to server for further operations
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("Error: Could not open socket. \n");
    }

    //signal(SIGINT, ctrlC_shutdown);
    server = gethostbyname(IPaddress);

    if (server == NULL) {
        printf("Error: No such host. \n");
        //return EXIT_FAILURE;
    }
    //memset(&serv_addr, '\0', sizeof(serv_addr));
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(portNum));

    socklen_t size = sizeof(serv_addr);

    //wait for 3 seconds
    while(connect(sockfd, (struct sockaddr *)&serv_addr,size)<0)
    {
        printf("Error: Could not connect. Trying Again In 3 Seconds\n");
        sleep(3);
    }

    printf("Connection Successful!\n");
    //Upon successful connection

   /******************** CHECK OUT *****************************/
    if((strcmp(argv[1], "checkout")) == 0){
        //./WTF checkout <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        char* projectName = argv[2];
        checkout(projectName, sock);)

        return EXIT_SUCCESS;
    }
    /******************** UPDATE *****************************/
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
          /****int sfd = open(sockfd,O_RDONLY);*/
          bzero(buffer,256);
          read(sockfd,buffer,255);
          //now buffer has the protocol Message - tokenize and decode

          char * token = strtok(buffer,":");
          int counter = 1;
          int length;
          char dirName[256];
          while(token != NULL){
            if(counter == 1 || counter == 3){
              length = atoi(token);
            } else if(counter == 2){
              /***sprintf(dirName, token);***/
            } else if(counter == 4){
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
        //read();

        return EXIT_SUCCESS;
    }
    /******************** UPGRADE *****************************/
    if((strcmp(argv[1], "upgrade")) == 0){
        //./WTF upgrade <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }


        return EXIT_SUCCESS;
    }
    /******************** COMMIT *****************************/
    if((strcmp(argv[1], "commit")) == 0){
        //./WTF commit <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }


        return EXIT_SUCCESS;
    }
    /******************** PUSH *****************************/
    if((strcmp(argv[1], "push")) == 0){
        //./WTF push <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }


        return EXIT_SUCCESS;
    }
    /******************** CREATE *****************************/
    if((strcmp(argv[1], "create")) == 0){
        //./WTF create <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }
        char* projectName = argv[2];
        int projectLength = strlen(projectName);

        //printf("%s\n",projectName);
        //printf("%d\n", projectLength);
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
        }
        //printf("%s\n",buffer);
        //Error handling an existing project
        if ((strcmp(buffer,"Error")) == 0){
            printf("Error: Project already exists. Please try a different filename.\n");
            return EXIT_FAILURE;
        }else{
            //otherwise server sends back the protocol for the manifest file.
            //create local version of the project and add this manafest file too it
            //create project file
            char projectPath[50];
            sprintf(projectPath,"./%s/",projectName);
            int check = mkdir(projectPath, 0700);
            if(check != 0){
                printf("Unable to make directory.\n");
            }
            //PROTOCOL  <number of bytes>:<contents of manifest>
            //make .Manifest file
            char manifestPath[50];
            sprintf(manifestPath,"./%s/Manifest",projectName);
            FILE *manifest = fopen(manifestPath, "w+");
            if (manifest == NULL){
                printf("Failed to create Manifest file for project.\n");
            }
            int versionNum = 1;
            fprintf(manifest,"%d\n",versionNum);
            fclose(manifest);
            printf("Local Repository successfully created. \n");
        }
        return EXIT_SUCCESS;
    }
    /******************** DESTROY *****************************/
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
          printf("Error: Could not write to socket. \n");
        } else {
          printf("Message sent.\n");
        }

        //read from socket - this confirms whether the task was completed or failed
        n = read(sockfd, buffer, 255);
        if(n < 0){
          printf("Error: Could not read from socket. \n");
        } else {
          printf("Message from server recieved. \n");
        }
        //check which type of error this is
        if(strcmp(buffer, "Project name doesn't exist") == 0){
          printf("Error: Project doesn't exist - can't delete %s.\n", projectName);
          return EXIT_FAILURE;
        } else if(strcmp(buffer, "Successfully deleted project") == 0){
          printf("Server successfully destroyed the project.\n");
        }

        return EXIT_SUCCESS;
    }
    /******************** CURRENT VERSION *****************************/
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
          printf("Error: Could not write to socket. \n");
        } else {
          printf("Message sent.\n");
        }

        //read message from socket
        n = read(sockfd, buffer, 255);
        if(n < 0){
          printf("Error: Could not read from socket. \n");
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
    /******************** HISTORY *****************************/
    if((strcmp(argv[1], "history")) == 0){
        //./WTF history <project name>
        if(argc != 3) {
            printf("Error: Invalid number of arguments. \n");
            return EXIT_FAILURE;
        }


        return EXIT_SUCCESS;
    }
    /******************** ROLLBACK *****************************/
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
