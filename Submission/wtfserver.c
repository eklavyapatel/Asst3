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

void* clientHandler (void *args);

//make the socket descriptor a global variable
//int sock_des = 0;


int remove_directory(const char *path){
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;
    if (d){
        struct dirent *p;
        r = 0;
        while (!r && (p=readdir(d))){
            int r2 = -1;
            char *buf;
            size_t len;
            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);
            if (buf){
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)){
                    if (S_ISDIR(statbuf.st_mode)){
                        r2 = remove_directory(buf);
                    }else{
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    else if(!d){
        return r;
    }
    
    if (!r){
        r = rmdir(path);
    }
    return r;
}

/*//method to check if a project exists
int exists(char *dir,char* project) {
    
    char* projectpath = (char*)malloc(9*sizeof(char));
    strcpy(projectpath, "./server/");
    strcat(projectpath, project);
    
    DIR *folder = opendir(projectpath);
    if(folder) {
        close(dir);
        printf("folder %s exists\n", projectpath);
        return 1;
    }
    printf("folder %s does not exist\n", projectpath);
    return 0;
}



//goes through a directory and gets all the files in it and its subdirectories
void traverseDir(char *dir) {
    
    DIR *folder;
    
    if(!(folder = opendir(dir))) {
        return;
    }
    
    struct dirent *curr = readdir(folder);
    curr = readdir(folder);
    
    while((curr = readdir(folder)) != NULL) {
        char pathway[1024];
        if(curr->d_type != DT_DIR){ //file
            int totalLen = strlen(curr->d_name)+1;
            char* temp1 = (char*)malloc(totalLen*sizeof(char));
            strcat(temp1, "/");
            strcat(temp1, curr->d_name);
            totalLen = strlen(dir) + strlen(temp1);
            char* temp = (char*)malloc(totalLen*sizeof(char));
            strcat(temp, dir);
            strcat(temp, temp1);
            
            //saves names of files in linked list
            if(fileNames == NULL){
                fileNames = (struct node*)malloc(1*sizeof(struct node));
                fileNames->name = temp;
                fileTrav = fileNames;
            } else {
                struct node* newPath = (struct node*)malloc(1*sizeof(struct node));
                newPath->name = temp;
                fileTrav->next =  newPath;
                fileTrav = fileTrav->next;
            }
            
        } else { //folder
            snprintf(pathway, sizeof(pathway), "%s/%s", dir, curr->d_name);
            traverseDir(pathway);
        }
    }
    close(folder);
}*/

//main function madafaka
int main(int argc, char *argv[]) {

    int portno;
    socklen_t clilen;

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
    char* repo = "./server_repo";
    int check = mkdir(repo, 0700);
    if(!check){
        printf("Repository Initiated.\n");
    }else{
        printf("Unable to create Repository.\n");
    }

    int sock_des = socket(AF_INET, SOCK_STREAM, 0);
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
                //printf("Front was pointing to NULL, now points to the first node.\n");
                front = newNode;
            }else{
                //printf("We added a new node to the LL\n");
                front = newNode;
                newNode->next = front;
                front = newNode;
            }
            pthread_create(&newNode->name, NULL, clientHandler, &client_socket);

            pthread_join(newNode->name, NULL);
        }
    }
    return EXIT_FAILURE;
}

//handles the client on a particular thread. must command calls go in here
void *clientHandler(void* client_socket){
    //printf("at client handle\n");
    int socket_num = *(int*) client_socket;

    char client_message[256];
    memset(client_message, '\0', sizeof(client_message));

    char send_message[256];
    memset(send_message, '\0', sizeof(send_message));

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
        //printf("Right before we tokenize.\n");
        char* command = strtok(client_message, s);

        //printf("%s\n", command);

        if((strcmp(command, "checkout")) == 0){
            /*if(exists("./server", project) != 1) {
                write(socket_num, "project not found", 18);
                return;
            }
            write(socket_num, "Sending project...\n", 43);
            //printf("project name %s\n", project);
            char* projectpath = (char*)malloc((14+strlen(project))*sizeof(char));
            projectpath[0] = '.';
            projectpath[1] = '/';
            projectpath[2] = 's';
            projectpath[3] = 'e';
            projectpath[4] = 'r';
            projectpath[5] = 'v';
            projectpath[6] = 'e';
            projectpath[7] = 'r';
            projectpath[8] = '_';
            projectpath[9] = 'r';
            projectpath[10] = 'e';
            projectpath[11] = 'p';
            projectpath[12] = 'o';
            projectpath[13] = '/';
            projectpath = strcat(projectpath, project);
            /printf("project path %s\n", projectpath);
            traverseDir(projectpath);
            
            char buffer[256];
            
            while(fileNames != NULL) {
                bzero(buffer, 256);
                //printf("linked list %s\n", fileNames->name);
                read(sd, buffer, 256);
                if(strcmp(buffer, "next") == 0) {
                    send(socket_num, fileNames->name, strlen(fileNames->name), 0);
                    bzero(buffer, 256);
                    read(socket_num, buffer, 256);
                    int fd = open(fileNames->name, O_RDONLY);
                    char* stringContent = "empty\a";
                    if(fsize(fileNames->name) >= 1) {
                        stringContent = readFile(fd, fileNames->name);
                        //printf("content %s\n", stringContent);
                    }
                    send(sd, stringContent, strlen(stringContent), 0);
                }
                fileNames = fileNames->next;
            }
            
            read(socket_num, buffer, 256);
            send(socket_num, "done", 5, 0);
            bzero(buffer, 256);
            close(socket_num);
        }*/
            //server-side stuff fore commit goes here
        }else if((strcmp(command, "update")) == 0){

        }else if((strcmp(command, "uprade")) == 0){

        }else if((strcmp(command, "commit")) == 0){

        }else if((strcmp(command, "push")) == 0){

        }else if((strcmp(command, "create")) == 0){
/************************** C R E A T E ***********************************************/
            //printf("command is create. \n");
            //recieves the protocol for create, decifer and execute.
            int projectNameLength = atoi(strtok(NULL,s));
            char* projectName = strtok(NULL,s);

            //printf ("%d\n",projectNameLength);
            //printf ("%s\n",projectName);

            char repoPath[50];
            sprintf(repoPath,"./server_repo/%s/",projectName);
            //printf("%s\n",repoPath);
            struct stat projectDir = {0};
            //now create the directory in repo
            if (stat(repoPath, &projectDir ) == -1) {
                int check = mkdir(repoPath, 0700);
                if(check != 0){
                    printf("Unable to make directory.\n");
                }
                //project file created. create version 1 file and manifest file
                char version1Path[50];
                sprintf(version1Path,"./server_repo/%s/version1",projectName);
                //printf("%s\n",version1Path);
                check = mkdir(version1Path, 0700);
                if(check != 0){
                    printf("Unable to make directory.\n");
                }
                //create manifest file in it
                char manifestPath[50];
                sprintf(manifestPath,"./server_repo/%s/version1/Manifest",projectName);
                //printf("%s\n",manifestPath);
                //"ab+" - Creates an empty file for both reading and writing.
                FILE *manifest = fopen(manifestPath, "w+");
                if (manifest == NULL){
                    printf("Failed to create Manifest file for project.\n");
                }
                int versionNum = 1;
                fprintf(manifest,"%d\n",versionNum);
                int manifestBytes = 2;
                char* manifestContent = "1\n";
                printf("Project: %s, successfully created.\n",projectName);
                //PROTOCOL  <number of bytes>:<contents of manifest>
                memset(send_message, '\0', sizeof(send_message));
                sprintf(send_message,"%d:%s",manifestBytes,manifestContent);
                fclose(manifest);
                //now send that shit over
                write(socket_num, send_message, sizeof(send_message));

            }else{
            //project already exists. send message to client
                //printf("directory exists.\n");
                write(socket_num, "Error", 6);
            }

        }else if((strcmp(command, "destroy")) == 0){
/************************** D E L E T E ***********************************************/
            printf("Command is destroy. \n");
            int nameLength;
            char* projectName;
            nameLength = atoi(strtok(NULL,s));
            projectName = strtok(NULL,s);
            //Step 1: Lock the repo

            //Step 2: Expire any pending commits

            //Step 3: Delete all files and subdirectories in the project directory
              //Substep 1: First create a path to the project in the server repo
            char deletebuff [256];
            strcpy(deletebuff, "./server_repo/");
            strcat(deletebuff, projectName);
            //printf("%s\n", deletebuff);
              //Substep 2: Open the directory and delete all children
            int check = remove_directory(deletebuff);

            //Step 4: Send success message
            if(check == 0 ){
              write(socket_num, "Successfully deleted project",28);
            } else if(check < 0){
              write(socket_num, "Project name doesn't exist",25);

            }

        }else if((strcmp(command, "currentversion")) == 0){
          //create the path to the desired project
          int nameLength = atoi(strtok(NULL,s));
          char * projectName = strtok(NULL,s);
          char cvbuff [256];
          sprintf(cvbuff, "./server_repo/%s", projectName);

          //Step 1: Open the directory
          DIR *d = opendir(cvbuff);
          struct dirent * file;
          if(d == NULL){
            write(socket_num, "Project does not exist.", 23);
            return;
          } else {
            while(file = readdir(d) != NULL){
              if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ){
                continue;
              }

            }
          }
          //Step 2: Look for greatest version
          //Step 3: Send that version and the manifest through
        }else if((strcmp(command, "history")) == 0){

        }else if((strcmp(command, "rollback")) == 0){

        }else{
            //if not valid command
        }
    }
    close(socket_num);

    return NULL;
}
