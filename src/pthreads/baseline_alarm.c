#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[argc+1]) {
  int sec;
  char line[128];
  char msg[64];

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
      sleep(sec);
      printf("(%d) %s\n", sec, msg);
    }
  }

}
