#include "hcr.h"

void minprintf(char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  int ival;
  double dval;

  va_start(ap, fmt);
  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }
    switch (*++p) {
    case 'd':
      ival = va_arg(ap, int);
      printf("%d", ival);
      break;
    case 'f':
      dval = va_arg(ap, double);
      printf("%f", dval);
      break;
    case 's':
      for (sval = va_arg(ap, char *); *sval; sval++)
        putchar(*sval);
      break;
    default:
      putchar(*p);
      break;
    }
  }
  va_end(ap);
}

void filecp(FILE *ifp, FILE *ofp) {
  int c;

  while ((c = getc(ifp)) != EOF)
    putc(c, ofp);
}

int cat(int argc, char **argv) {
  FILE *fp;
  if (argc == 1) {
    filecp(stdin, stdout);
  } else {
    while (--argc > 0) {
      if ((fp = fopen(*++argv, "r")) == NULL) {
        fprintf(stderr, "cat: cannot open %s\n", *argv);
        return 1;
      } else {
        filecp(fp, stdout);
        fclose(fp);
      }
    }
  }
  return 0;
}