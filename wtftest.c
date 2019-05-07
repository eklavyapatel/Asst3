#inlcude "wtf.h"

int main(int argc, char**argv){
    char* exampleName = "Example1";
    exec("./wtfclient configure cp.cs.rutgers.edu 8080");
    exec("./wtfclient create Example1");
    exec("./wtfclient add File1");
    exec("./wtfclient add File2");
    exec("./wtfclient commit Example1");
    exec("./wtfclient push Example1");
    exec("./wtfclient remove File2");
    exec("./wtfclient update Example1");
    exec("./wtfclient upgrade Example1");
    exec("./wtfclient history Example1");
    exec("./wtfclient currentversion Example1");
    exec("./wtfclient rollback Example1 1");
    exec("./wtfclient destroy Example1");

}
