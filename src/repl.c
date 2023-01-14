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

void flushall() {
  fflush(stdout);
  fflush(stderr);
}

void prompt() {
  fputs(ANSI_COLOR_CYAN "% " ANSI_RESET_ALL, stdout);
  flushall();
}

void registerSIGHNDs();

void eval(const char *cmdline) {
  char *argv[MAXARGS];
  int argc;
  pid_t pid;

  parseline(cmdline, argv, &argc);
  if ((pid = Fork()) == 0) {
    sfindFunc(argv[0])(argc, argv);
  }
  wait(NULL);
}

int main() {
  printf("~ init REPL\n");
  registerSIGHNDs();

  CharBuffer cbShellInput;
  initCharBuffer(&cbShellInput);
  while (prompt(), readCharBuffer(&cbShellInput, stdin) > -1) {
    eval(cbShellInput.cBuffer);
    // fputs(cbShellInput.cBuffer, stdout);
  }
  freeCharBuffer(&cbShellInput);
  flushall();

  return 0;
}