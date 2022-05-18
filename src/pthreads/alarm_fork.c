#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char *argv[argc+1]) {
  int sec;
  char line[128];
  char msg[64];
  pid_t pid;

  while(1) {
    printf("Alarm> ");

    if(fgets(line, sizeof(line), stdin) == NULL)
      exit(0);

    if(strlen(line) <= 1)
      continue;

    // parse input line into seconds (%d) and a message
    // (%64[^\n]), consisting of up to 64 characters
    // separated from the seconds by whitespace

    if(sscanf(line, "%d %s", &sec, msg) < 2) {
      fprintf(stderr, "Bad command\n" );

    } else {
      pid = fork();
      if(pid == (pid_t) - 1)
        exit(1); 
      
      if(pid == (pid_t) 0) {
        // in the child, wait and then print a message
        sleep(sec);
        printf("\n(%d) %s\n", sec, msg);
        exit(0);
      } else {
        // in the paraent, call waitpid() to collect children
        // that have already terminated.

        do {
          pid = waitpid((pid_t) -1, NULL, WNOHANG);
          if(pid == (pid_t) - 1)
            exit(1); 
        } while(pid != (pid_t) 0);
      }
    }
  }
}
