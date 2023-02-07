// dcla.c: a recursive descend grammar analyzer
// for declarators
//

#include "hcr.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

enum {
  NAME,
  PARENS,
  BRACKETS,
  MAXTOKEN = 100,
  MAXOUT = 8192,
};

void dcl(void);
void dirdcl(void);

static int gettoken(void);
static int tokentype;
static char token[MAXTOKEN];
static char name[MAXTOKEN];
static char datatype[MAXTOKEN];
static char out[MAXOUT];

void dcl(void) {
  int ns;

  // dcl: optional *'s direct-dcl
  for (ns = 0; gettoken() == '*'; ns++)
    ;
  dirdcl();
  while (ns-- > 0)
    strcat(out, " pointer to");
}

void dirdcl(void) {
  int type;

  // direct-dcl: (dcl)
  if (tokentype == '(') {
    dcl();
    if (tokentype != ')')
      printf("error: missing )\n");
  } else if (tokentype == NAME) {
    // name
    strcpy(name, token);
  } else {
    printf("error: expected name or (dcl)\n");
  }

  while ((type = gettoken()) == PARENS || type == BRACKETS) {
    // direct-dcl ()
    if (type == PARENS) {
      strcat(out, " function returning");
    } else {
      // direct-dcl []
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
  }
}

int dcla() {
  while (gettoken() != EOF) {
    strcpy(datatype, token);
    out[0] = '\0';
    dcl();
    if (tokentype != '\n') {
      printf("syntax error\n");
    }
    printf("%s: %s %s\n", name, out, datatype);
  }
  return 0;
}

// buffer
static char buf[MAXOUT];
static int buflen = 0;

static int getch() { return (buflen > 0) ? buf[--buflen] : getchar(); }

static void ungetch(char c) { buf[buflen++] = c; }

int gettoken(void) {
  int c;
  char *p = token;

  while ((c = getch()) == ' ' || c == '\t')
    ;
  if (c == '(') {
    if ((c = getch()) == ')') {
      strcpy(token, "()");
      return tokentype = PARENS;
    } else {
      ungetch(c);
      return tokentype = '(';
    }
  } else if (c == '[') {
    for (*p++ = c; (*p++ = getch()) != ']';)
      ;
    *p = '\0';
    return tokentype = BRACKETS;
  } else if (isalpha(c)) {
    for (*p++ = c; isalnum(c = getch()); *p++ = c)
      ;
    *p = '\0';
    ungetch(c);
    return tokentype = NAME;
  } else {
    return tokentype = c;
  }
}

void cmd_dcla(int argc, char **argv) {
  assert(argc == 1);
  assert(argv != NULL);
  exit(dcla());
}
