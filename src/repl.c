#include "repl.h"

ssize_t Getline(char **area, size_t *n, FILE *f) {
  ssize_t inputsz;

  if ((inputsz = getline(area, n, f)) < 0) {
    unix_warning("Getline No Free Space");
    return -1;
  }

  return inputsz;
}

int initCharBuffer(CharBuffer *cb) {
  return cb->i32uBufferSize = cb->i32uInputSize = 0;
}

int readCharBuffer(CharBuffer *cb, FILE *fs) {
  ssize_t n;

  if ((cb->i32uInputSize = Getline(&(cb->cBuffer), &(cb->i32uBufferSize), fs)) <
      0) {
    unix_warning("readCharBuffer No Free Space");
    return -1;
  }

  return 0;
}

void freeCharBuffer(CharBuffer *cb) { free(cb->cBuffer); }

void prompt() { fputs("% ", stdout); }

int main() {
  printf("hello, world.\n");
  CharBuffer cbShellInput;
  initCharBuffer(&cbShellInput);
  while (prompt(), readCharBuffer(&cbShellInput, stdin) > -1) {
    fputs(cbShellInput.cBuffer, stdout);
  }
  freeCharBuffer(&cbShellInput);

  return 0;
}