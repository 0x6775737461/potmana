#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>

typedef struct alarm_tag {
  int sec;
  char msg[64];
} alarm_t;

void *alarm_thread(void *arg) {
  alarm_t *alarm = (alarm_t *) arg;
  int status;

  status = pthread_detach (pthread_self());
  if (status != 0) {
    fprintf(stderr, "Detach thread");
    exit(status);
  }

  sleep (alarm->sec);
  printf("(%d) [%s]\n", alarm->sec, alarm->msg);

  free(alarm);

  return NULL;
}

int main(int argc, char *argv[argc+1]) {
  int status;
  char line[128];
  alarm_t *alarm;
  pthread_t thread;

  while(1) {
    printf("Alarm> ");

    if(fgets(line, sizeof(line), stdin) == NULL)
      exit(0);

    if(strlen(line) <= 1)
      continue;

    alarm = (alarm_t *) malloc (sizeof(alarm_t));
    if(alarm == NULL) {
      fprintf(stderr, "Allocate alarm\n");
      exit(1);
    }
      
    // parse input line into seconds (%d) and a message
    // (%64[^\n]), consisting of up to 64 characters
    // separated from the seconds by whitespace

    if(sscanf(line, "%d %s", &alarm->sec, alarm->msg) < 2) {
      fprintf(stderr, "Bad command\n" );
      free(alarm);

    } else {
      status = pthread_create(
        &thread, NULL, alarm_thread, alarm);

      if (status != 0) {
        fprintf (stderr, "Create alarm thread\n");
        exit(1);
      }
    }
  }
}
