#include "repl.h"

const size_t MAXLINE = 1024;
const size_t MAXARGS = 32;

char *lexer(char *iterator, char **delim) {
  for (; (*iterator) == ' '; iterator++)
    ;
  if (*iterator == '\'') {
    iterator++;
    *delim = strchr(iterator, '\'');
  } else {
    *delim = strchr(iterator, ' ');
  }
  return iterator;
}

int parseline(const char *cmdline, char **argv, int *argc) {
  int cmdlineLen = strlen(cmdline);
  char array[MAXLINE];
  char *iterator = array;
  char *delim;

  strcpy(iterator, cmdline);

  /* build the argv list */
  int t_argc = 0;
  iterator[cmdlineLen - 1] = ' ';
  iterator = lexer(iterator, &delim);

  for (; delim != NULL; iterator = lexer(iterator, &delim)) {
    argv[t_argc++] = iterator;
    *delim = '\0';
    iterator = delim + 1;
  }

  argv[t_argc] = NULL;
  if ((*argc = t_argc) == 0)
    return 1;

  return 0;
}

commonFunc *sfindFunc(const char *name) {
  if (!strcmp(name, "cchecker")) {
    return cmd_CChecker;
  } else if (!strcmp(name, "csim")) {
    return cmd_csim;
  } else if (!strcmp(name, "hello")) {
    return cmd_hello;
  }

  return NULL;
}