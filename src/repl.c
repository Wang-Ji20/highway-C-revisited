#include "repl.h"

ssize_t Getline(char **area, size_t *n, FILE *f) {
  ssize_t inputsz;

  if ((inputsz = getline(area, n, f)) < 0) {
    if (errno)
      unix_panic("Getline No Free Space");
    return -1;
  }

  return inputsz;
}

int initCharBuffer(CharBuffer *cb) {
  return cb->sztBufferSize = cb->ssztBufferSize = 0;
}

int readCharBuffer(CharBuffer *cb, FILE *fs) {
  ssize_t n;

  if ((cb->ssztBufferSize = Getline(&(cb->cBuffer), &(cb->sztBufferSize), fs)) <
      0) {
    if (errno)
      unix_panic("readCharBuffer No Free Space");
    fprintf(stderr, "\nBye\n");
    return -1;
  }

  return 0;
}

void freeCharBuffer(CharBuffer *cb) { free(cb->cBuffer); }

void prompt() { fputs(ANSI_COLOR_CYAN "% " ANSI_RESET_ALL, stdout); }

/* signal handlers */

void sigint_handler(int sig) { write(STDERR_FILENO, "SIGINT\n", 8); }

void registerSIGHNDs() { Signal(SIGINT, sigint_handler); }

int main() {
  printf("~ init REPL\n");
  registerSIGHNDs();

  CharBuffer cbShellInput;
  initCharBuffer(&cbShellInput);
  while (prompt(), readCharBuffer(&cbShellInput, stdin) > -1) {
    fputs(cbShellInput.cBuffer, stdout);
  }
  freeCharBuffer(&cbShellInput);

  return 0;
}