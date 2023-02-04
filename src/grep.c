#include "hcr.h"
#include <stdlib.h>

/* this piece of code comes from The Practice of Programming
 *   by Rob Pike.
 */
int match(char *regexp, char *text);
int matchhere(char *regexp, char *text);
int matchstar(int c, char *regexp, char *text);

// search for c*regexp at beginning of text
int matchstar(int c, char *regexp, char *text) {
  do {
    if (matchhere(regexp, text))
      return -1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}

// search for regexp at beginning of text
int matchhere(char *regexp, char *text) {
  if (regexp[0] == '\0')
    return 1;
  if (regexp[1] == '*')
    return matchstar(regexp[0], regexp + 2, text);
  if (regexp[0] == '$' && regexp[1] == '\0')
    return *text == '\0';
  if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
    return matchhere(regexp + 1, text + 1);
  return 0;
}

// search for regexp anywhere in text;
int match(char *regexp, char *text) {
  if (regexp[0] == '^')
    return matchhere(regexp + 1, text);
  do {
    if (matchhere(regexp, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

// The following code comes from K & R

static const int MAXLINE = 1000;

int readline(char s[], int lim) {
  int c, i = 0;
  while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = 0;
  return i;
}

int strindex(char s[], char t[]) {
  int i, j, k;
  for (i = 0; s[i] != '\0'; i++) {
    for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
      ;
    if (k > 0 && t[k] == '\0')
      return i;
  }
  return -1;
}

int grep(char *pattern) {
  char line[MAXLINE];
  int found = 0;
  while (readline(line, MAXLINE) > 0) {
    if (match(pattern, line) == 1) {
      printf("%s", line);
      found++;
    }
  }
  return found;
}

void cmd_grep(int argc, char **argv) {
  grep(argv[argc - 1]);
  exit(EXIT_SUCCESS);
}