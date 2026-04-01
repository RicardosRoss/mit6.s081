#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char*argv[]){
    if(argc<2)
    {
        write(1,"ERROR!\n",strlen("ERROR!"));
        exit(1);
    }
    int sleepTime=atoi(argv[1]);
 sleep(sleepTime);
    exit(0);
}