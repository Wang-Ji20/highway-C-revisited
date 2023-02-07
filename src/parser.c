// This file contains a simple lexer & parser for command line
// arguments processing.
// They removes all redundant spaces and trailing '\n', then
// save arguments in argv, and arguments' count in argc.
// NB. string between a pair of single quote is regarded as
// one argument.

#include "hcr.h"
#include "repl.h"
#include <string.h>

const size_t MAXLINE = 1024;
const size_t MAXARGS = 32;

/* lexer read byte string from iterator
 *  return the first element of token
 *  set delim pointer to the last element of tokrn
 */
char *lexer(char *iterator, char **delim) {
  /* skip all spaces */
  for (; (*iterator) && (*iterator) == ' '; iterator++)
    ;

  if (*iterator == '\0') {
    *delim = NULL;
    return NULL;
  }

  /* if ' get the next ' */
  if (*iterator == '\'') {
    iterator++;
    *delim = strchr(iterator, '\'');
  } else {
    *delim = strchr(iterator, ' ');
  }

  /* token is a string. so add \0 */
  **delim = '\0';

  fdebug("get a token %s\n", iterator);

  return iterator;
}

int parseline(char *cmdline, char **argv, int *argc) {
  int cmdlineLen = strlen(cmdline);
  char *iterator = cmdline;
  char *delim;

  fdebug("start parse line length %d:\n%s", cmdlineLen, iterator);

  /* build the argv list */
  int t_argc = 0;
  iterator[cmdlineLen - 1] = ' ';
  iterator = lexer(iterator, &delim);

  for (; delim != NULL; iterator = lexer(iterator, &delim)) {
    argv[t_argc++] = iterator;
    iterator = delim + 1;
  }

  argv[t_argc] = NULL;
  if ((*argc = t_argc) == 0)
    return 1;

  for (int i = 0; i < t_argc; i++) {
    fdebug("argv[%d] size %d : %s\n", i, strlen(argv[i]), argv[i]);
  }

  return 0;
}

commonFunc *sfindFunc(const char *name) {
  fdebug("check command for %s\n", name);
  if (!strcmp(name, "cchecker")) {
    return cmd_CChecker;
  } else if (!strcmp(name, "csim")) {
    return cmd_csim;
  } else if (!strcmp(name, "hello")) {
    return cmd_hello;
  } else if (!strcmp(name, "grep")) {
    return cmd_grep;
  } else if (!strcmp(name, "revpol")) {
    return cmd_revpol;
  } else if (!strcmp(name, "dcla")) {
    return cmd_dcla;
  }

  fdebug("nomatch!\n");

  return NULL;
}