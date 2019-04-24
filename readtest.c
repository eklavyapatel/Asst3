#include "wtf.h"




int main(){
    char* IPaddress;
    int portNum;
    int fd = open("configure", O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        if (errno == EEXIST) {
            printf("file found! \n");
            close(fd);
            int fd = open("configure", O_RDONLY );
            if(fd <0){
                printf("sumthin wong");
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
    }else{
         printf("check again \n");
    }
}
    
