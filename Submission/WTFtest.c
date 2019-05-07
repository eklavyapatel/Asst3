#inlcude "wtf.h"

int main(int argc, char**argv){
    pid_t child = fork();
    if(child < 0){
        perror("fork() error\n");
        exit(-1);
    }
    if(child != 0){
        wait(NULL);
    } else {
      execvp("./wtfclient", "configure", "cp.cs.rutgers.edu", "8080");
      sleep();
      execvp("./wtfclient", "create", "Example1");
      sleep();
      execvp("./wtfclient", "add", "File1");
      sleep();
      execvp("./wtfclient", "add", "File2");
      sleep();
      execvp("./wtfclient", "commit", "Example1");
      sleep();
      execvp("./wtfclient", "push", "Example1");
      sleep();
      execvp("./wtfclient", "remove", "File2");
      sleep();
      execvp("./wtfclient", "update", "Example1");
      sleep();
      execvp("./wtfclient", "upgrade", "Example1");
      sleep();
      execvp("./wtfclient", "history", "Example1");
      sleep();
      execvp("./wtfclient", "currentversion", "Example1");
      sleep();
      execvp("./wtfclient", "rollback", "Example1", "1");
      sleep();
      execvp("./wtfclient", "destroy", "Example1");
      sleep();
    }

return 0;
}
