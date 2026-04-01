#include "kernel/types.h"
#include "user/user.h"

void sieve(int read_fd) {
      int prime;
      // 读第一个数 → 一定是素数
      if (read(read_fd, &prime, 4) == 0) {
          close(read_fd);
          exit(0);
      }
      printf("prime %d\n", prime);

      int p[2];
      int has_child = 0;
      int num;

      // 继续读后续的数
      while (read(read_fd, &num, 4) > 0) {
          if (num % prime != 0) {
              // 不能被 prime 整除，传给下一级
              if (!has_child) {
                  pipe(p);  //只在需要时才创建下一级管道
                  if (fork() == 0) {
                      // 子进程：从新管道读
                      close(p[1]);
                      close(read_fd);
                      sieve(p[0]);  //递归：做同样的事
                  }
                  close(p[0]);
                  has_child = 1;
              }
              write(p[1], &num, 4);
          }
          // 能整除 → 直接丢弃
      }

      close(read_fd);
      if (has_child) {
          close(p[1]);    // 关闭写端 →通知子进程"没数据了"
          wait(0);        // 等子进程结束
      }
      exit(0);
  }

int main(int argc, char *argv[]) {
      int p[2];
      pipe(p);

      if (fork() == 0) {
          // 子进程：从管道读，开始筛
          close(p[1]);
          sieve(p[0]);
      } else {
          // 父进程：往管道喂 2~35
          close(p[0]);
          for (int i = 2; i <= 35; i++) {
              write(p[1], &i, 4);
          }
          close(p[1]);   // 关闭写端 → 子进程read 返回 0
          wait(0);       // 等整个流水线结束
      }

      exit(0);
  }