#include "hcr.h"
#include "repl.h"

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

int parseline(const char *cmdline, char **argv, int *argc) {
  int cmdlineLen = strlen(cmdline);
  char array[MAXLINE];
  char *iterator = array;
  char *delim;

  strcpy(iterator, cmdline);
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
    fdebug("argv[%d]: %s\n", i, argv[i]);
  }

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