#include "wtf.h"
#include <sys/wait.h>

int main(int argc, char**argv){
    pid_t child = fork();
    if(child < 0){
        perror("fork() error\n");
        exit(-1);
    }
    if(child != 0){
        wait(NULL);
    } else {
      execlp("./wtfclient", "configure", "cp.cs.rutgers.edu", "8080");
      sleep(5);
      execlp("./wtfclient", "create", "Example1");
      sleep(5);
      execlp("./wtfclient", "add", "File1");
      sleep(5);
      execlp("./wtfclient", "add", "File2");
      sleep(5);
      execlp("./wtfclient", "commit", "Example1");
      sleep(5);
      execlp("./wtfclient", "push", "Example1");
      sleep(5);
      execlp("./wtfclient", "remove", "File2");
      sleep(5);
      execlp("./wtfclient", "update", "Example1");
      sleep(5);
      execlp("./wtfclient", "upgrade", "Example1");
      sleep(5);
      execlp("./wtfclient", "history", "Example1");
      sleep(5);
      execlp("./wtfclient", "currentversion", "Example1");
      sleep(5);
      execlp("./wtfclient", "rollback", "Example1", "1");
      sleep(5);
      execlp("./wtfclient", "destroy", "Example1");
      sleep(5);
    }

return 0;
}
