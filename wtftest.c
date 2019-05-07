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
      exec("./wtfclient configure cp.cs.rutgers.edu 8080");
      sleep();
      exec("./wtfclient create Example1");
      sleep();
      exec("./wtfclient add File1");
      sleep();
      exec("./wtfclient add File2");
      sleep();
      exec("./wtfclient commit Example1");
      sleep();
      exec("./wtfclient push Example1");
      sleep();
      exec("./wtfclient remove File2");
      sleep();
      exec("./wtfclient update Example1");
      sleep();
      exec("./wtfclient upgrade Example1");
      sleep();
      exec("./wtfclient history Example1");
      sleep();
      exec("./wtfclient currentversion Example1");
      sleep();
      exec("./wtfclient rollback Example1 1");
      sleep();
      exec("./wtfclient destroy Example1");
      sleep();
    }

return 0;
}
