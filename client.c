#include "wtf.h"

int firstRun = 1;

void configure(char* address, char* port){
    int fd_configure = open("./.configure", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_configure < 0) {
        printf("Error: Could not create configure file. Please try again.\n");
        return EXIT_FAILURE;
    }
    write(fd_configure, address, strlen(address));
    write(fd_configure, "\t", 1);
    write(fd_configure, port, strlen(port));
    
    close(fd_configure);
    return;
}

int main(int argc, char** argv){
    struct sockaddr_in serverinfo;
    struct hostent *serverip;
    
    signal(SIGINT, ctrlc_shutdown);
    
    //if this is the first run of the client program, execute configure
    if(firstRun){
        //check for sufficient arguments
        if(argc != 3){
            printf("ERROR: Insufficient arguments to configure.");
            return EXIT_FAILURE;
        }
        //first command must be configure
        if(!strcmp(argv[1],"configure")){
            printf("ERROR: First command must be configure");
            return EXIT_FAILURE;
        }
        
        char* IPaddr = argv[2];
        char* portNum = argv[3];
        
        //call congifure to create a file to store input values
        configure(IPaddr, portNum);
        
        firsRun = 0;
        return EXIT_SUCCESS;
    }
    
    if(pthread_mutex_init(&lock, NULL)!=0){
        printf("ERROR: Failed creating mutex \n");
    }

    if(strcmp(arg[1],"add")){
        if(argc != 3){
            printf("ERROR: Insufficient arguments to add file.");
            return EXIT_FAILURE;
        }
        struct dirent *de;
        DIR *dr;
        dr = opendir(argv[2]);
        if(dr == NULL){
            printf("ERROR: Project does not exist on client.");
            return EXIT_FAILURE;
        }
        //create a file of input name and add it to open directory
        int newFile = open( argv[3] , O_WRONLY | O_CREAT | O_TRUNC, 0644);
        //oepn the manifest file
        int fd_manifest = open(".Manifest", O_WRONLY | O_APPEND);
        write( fd_manifest, "1",1);
        write( fd_manifest, "\t",1);
        write( fd_manifest, argv[3] , strlen(argv[3]));
        write( fd_manifest, "\t",1);
        
        
    }
    if(strcmp(arg[1],"remove")){
        
    }
    
    
    
    
    
    serverip =
    return 0;
}

