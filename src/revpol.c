#include "hcr.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  MAXOP = 100,
  NUMBER = '0',
  MAXVAL = 100,
};

static int sp = 0;
static double val[MAXVAL];

static int getop(char[]);
static void push(double);
static double pop(void);

void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else {
    printf("error: stack full, can't push %g\n", f);
  }
}

double pop(void) {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty");
  }
  return 0.0;
}

int revpol() {
  int type;
  double op2;
  char s[MAXOP];

  while ((type = getop(s)) != EOF) {
    switch (type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      push(pop() + pop());
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      // because evaluation order is undefined?
      op2 = pop();
      push(pop() - op2);
      break;
    case '/':
      op2 = pop();
      if (op2 == 0.0) {
        printf("error: zero divisor\n");
        return EXIT_FAILURE;
      }
      push(pop() / op2);
      break;
    case '\n':
      printf("\t%.8g\n", pop());
      break;
    default:
      printf("error: unknown command %s", s);
      break;
    }
  }
  return 0;
}

/*
    IO functions
*/

enum { BUFSIZE = 100 };

static char buf[BUFSIZE];
static int bufp = 0;

int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }

void ungetch(int c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many char\n");
  else
    buf[bufp++] = c;
}

int getop(char s[]) {
  int i, c;
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  s[1] = '\0';
  if (!isdigit(c) && c != '.')
    return c;
  i = 0;
  if (isdigit(c))
    while (isdigit(s[++i] = c = getch()))
      ;
  if (c == '.')
    while (isdigit(s[++i] = c = getch()))
      ;
  s[i] = '\0';
  if (c != EOF)
    ;
  ungetch(c);
  return NUMBER;
}

void cmd_revpol(int argc, char **argv) { exit(revpol()); }