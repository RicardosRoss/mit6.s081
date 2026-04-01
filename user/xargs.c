#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char buf[512];
  char *args[MAXARG];
  int i, n;

  if (argc < 2)
  {
    fprintf(2, "Usage: xargs command [args...]\n");
    exit(1);
  }

  for (i = 1; i < argc && i < MAXARG - 1; i++)
  {
    args[i - 1] = argv[i];
  }

  int pos = 0;
  while ((n = read(0, buf + pos, 1)) > 0)
  {
    if (buf[pos] == '\n')
    {
      buf[pos] = '\0';
      // Add the line as the last argument
      args[argc - 1] = buf;
      args[argc] = 0;

      if (fork() == 0)
      {
        exec(args[0], args);
        fprintf(2, "xargs: exec %s failed\n", args[0]);
        exit(1);
      }
      wait(0);
      pos = 0;
    }
    else
    {
      pos++;
    }
  }

  if (pos > 0)
  {
    buf[pos] = '\0';
    args[argc - 1] = buf;
    args[argc] = 0;
    if (fork() == 0)
    {
      exec(args[0], args);
      fprintf(2, "xargs: exec %s failed\n", args[0]);
      exit(1);
    }
    wait(0);
  }

  exit(0);
}
