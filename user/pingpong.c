#include "kernel/types.h"
#include "user/user.h"

int main(){
    int parent[2],child[2];
    char buf[100];
    int pid;
    pipe(parent);
    pipe(child);
    pid=fork();
    if (pid == 0) {
      close(parent[1]);  
      close(child[0]);   
      read(parent[0], buf, 1);
      printf("%d: received ping\n", getpid());
      write(child[1], buf, 1);
      close(parent[0]);
      close(child[1]);
      exit(0);
  } else {
      close(parent[0]); 
      close(child[1]);  
      write(parent[1], "x", 1);
      close(parent[1]);
      read(child[0], buf, 1);
      printf("%d: received pong\n", getpid());
      close(child[0]);
      exit(0);
  }
}