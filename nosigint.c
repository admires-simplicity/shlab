#include <stdio.h>
#include <signal.h>	
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

typedef void handler_t(int);

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
  fprintf(stdout, "%s: %s\n", msg, strerror(errno));
  exit(1);
}


/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
  struct sigaction action, old_action;

  action.sa_handler = handler;  
  sigemptyset(&action.sa_mask); /* block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
  return (old_action.sa_handler);
}


void sigint_handler(int sig) 
{
  printf("not responding to sigint handler. AND calling printf in a signal hander, which is unsafe.\n");
  return;
}

int main() {
  Signal(SIGINT,  sigint_handler);   /* ctrl-c */

  printf("sleeping for 10 seconds\n");
  time_t start = time(NULL);
  while (time(NULL) - start < 10) {
    printf("sleeping for 1 second\n");
    sleep(1);
  }
  printf("terminated normally\n");
  return 0;
}
