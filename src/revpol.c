#include "hcr.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  MAXOP = 100,
  NUMBER = '0',
  MAXVAL = 100,
  IDENTIFIER = 'a',
};

int sp = 0;
double val[MAXVAL];
double env[28] = {[0 ... 27] = NAN};

static int getop(char[]);
static double push(double);
static double pop(void);

double push(double f) {
  if (sp < MAXVAL)
    return val[sp++] = f;
  printf("error: stack full, can't push %g\n", f);
  return NAN;
}

double pop(void) {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty");
  }
  return NAN;
}

static double top(void) {
  if (sp < 1) {
    printf("error: stack empty");
    return 0.0;
  }
  return val[sp];
}

static double sdup(void) {
  if (sp < 1) {
    printf("error: stack empty");
    return 0.0;
  }
  return val[sp++] = top();
}

static int builtin(char s[]) {
  double op2;

  if (!strcmp("sin", s)) {
    push(sin(pop()));
  } else if (!strcmp("exp", s)) {
    push(exp(pop()));
  } else if (!strcmp("pow", s)) {
    op2 = pop();
    push(pow(pop(), op2));
  } else {
    // not built-in function
    return 0;
  }

  return 1;
}

static double calc_iden(char s[]) {
  double res = 0;
  if ((res = builtin(s)))
    return res;
  // variable !
  if (strlen(s) > 1) {
    fpanic("variable %s length exceed", s);
  }

  int idx = s[0] - IDENTIFIER;

  if (isnan(env[idx])) {
    res = pop();
    printf("assigned %g to %c\n", res, s[0]);
    return env[idx] = res;
  }

  printf("var %c is %g\n", s[0], env[idx]);

  return push(env[idx]);
}

int revpol() {
  int type;
  double op2;
  char s[MAXOP];

  while ((type = getop(s)) != EOF) {
    switch (type) {
    case IDENTIFIER:
      calc_iden(s);
      break;
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
    case '%':
      op2 = pop();
      push((int)round(pop()) % (int)round(op2));
      break;
    case '#':
      printf("duplicated %g", sdup());
      break;
    case '@':
      printf("top is %g", top());
      break;
    case '\n':
      if (isnan(op2 = pop()))
        env[27] = op2;
      printf("\t%.8g\n", env[27]);
      break;
    default:
      printf("error: unknown command %s\n", s);
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
  // for single operator, '\0' is needed
  // for number/tokens, it will be rewritten.
  s[1] = '\0';
  // char token
  i = 0;
  if (!isdigit(c) && c != '.') {
    // operator
    if (!isalpha(c))
      return c;
    // identifier
    while (isalpha(s[++i] = c = getch()))
      ;
    if (c != EOF)
      ungetch(c);
    s[i] = '\0';
    return IDENTIFIER;
  }
  // number
  i = 0;
  if (isdigit(c))
    while (isdigit(s[++i] = c = getch()))
      ;
  if (c == '.')
    while (isdigit(s[++i] = c = getch()))
      ;
  s[i] = '\0';
  if (c != EOF)
    ungetch(c);
  return NUMBER;
}

void cmd_revpol(int argc, char **argv) {
  argc = argv[1][0];
  argc--;
  exit(revpol());
}
