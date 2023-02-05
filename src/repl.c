// This file contains a simple interpreter.
#define _GNU_SOURCE
#include <errno.h>

#include "hcr.h"
#include "repl.h"
#include <bits/getopt_core.h>
#include <stdio.h>
#include <stdlib.h>

/* global variable showing whether we are in debug mode. */
int m_debug = 0;

/* wrapper for getline library function */
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
  return cb->sztBufferCap = cb->ssztBufferSize = 0;
}

int readCharBuffer(CharBuffer *cb, FILE *fs) {

  if ((cb->ssztBufferSize = Getline(&(cb->cBuffer), &(cb->sztBufferCap), fs)) <
      0) {
    if (errno)
      unix_panic("readCharBuffer No Free Space");
    fprintf(stderr, "\nBye\n");
    return -1;
  }

  return 0;
}

void freeCharBuffer(CharBuffer *cb) { free(cb->cBuffer); }

void prompt() {
  fputs(ANSI_COLOR_CYAN "% " ANSI_RESET_ALL, stdout);
  fflush(stdout);
}

void registerSIGHNDs();

int builtinCmd(const char *cmd) {
  if (!strcmp(cmd, "quit")) {
    exit(EXIT_SUCCESS);
    return 1;
  }
  return 0;
}

/* Evaluate input */
void eval(char *cmdline) {
  char *argv[MAXARGS];
  int argc;
  pid_t pid;

  if (parseline(cmdline, argv, &argc))
    return;

  if (builtinCmd(argv[0]))
    return;

  if ((pid = Fork()) == 0) {
    fdebug("forked!\n");
    sfindFunc(argv[0])(argc, argv);
    fflush(stdout);
  }
  fdebug("child pid %d!\n", pid);
  Wait(NULL);
  fflush(stdout);
}

/* an infinite loop. terminates on EOF */
void repl() {
  CharBuffer cbShellInput;
  initCharBuffer(&cbShellInput);
  while (prompt(), readCharBuffer(&cbShellInput, stdin) > -1) {
    eval(cbShellInput.cBuffer);
    // fputs(cbShellInput.cBuffer, stdout);
  }
  freeCharBuffer(&cbShellInput);
  fflush(stdout);
}

int main(int argc, char **argv) {
  int opt;

  /* init */
  printf("~ init REPL\n");
  registerSIGHNDs();

  /* get options */
  while ((opt = getopt(argc, argv, "d")) != -1) {
    switch (opt) {
    case 'd':
      m_debug = 1;
      break;
    default:
      printf("%s: unknown command %c.\n", program_invocation_short_name, opt);
      break;
    }
  }

  repl();

  return 0;
}