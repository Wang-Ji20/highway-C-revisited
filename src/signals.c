#include "repl.h"

handler_t *Signal(int signum, handler_t *handler) {
  struct sigaction action, oldaction;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  if (sigaction(signum, &action, &oldaction) < 0) {
    unix_panic("Signal error");
  }
  return oldaction.sa_handler;
}
/* signal handlers */

void sigint_handler(int sig) {
  assert(sig == SIGINT);
  write(STDERR_FILENO, "SIGINT\n", 8);
}

void registerSIGHNDs() { Signal(SIGINT, sigint_handler); }
