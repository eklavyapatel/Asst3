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
      execlp("./WTF", "configure", "cp.cs.rutgers.edu", "8080");
      sleep(5);
      execlp("./WTF", "create", "Example1");
      sleep(5);
      execlp("./WTF", "add", "File1");
      sleep(5);
      execlp("./WTF", "add", "File2");
      sleep(5);
      execlp("./WTF", "commit", "Example1");
      sleep(5);
      execlp("./WTF", "push", "Example1");
      sleep(5);
      execlp("./WTF", "remove", "File2");
      sleep(5);
      execlp("./WTF", "update", "Example1");
      sleep(5);
      execlp("./WTF", "upgrade", "Example1");
      sleep(5);
      execlp("./WTF", "history", "Example1");
      sleep(5);
      execlp("./WTF", "currentversion", "Example1");
      sleep(5);
      execlp("./WTF", "rollback", "Example1", "1");
      sleep(5);
      execlp("./WTF", "destroy", "Example1");
      sleep(5);
    }

return 0;
}
