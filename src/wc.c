#include "hcr.h"

/* wc: a classical word count program */
void wc(FILE *fin) {
  int lines = 0, words = 0, chars = 0, c = 0, state = WC_OUT;

  while ((c = fgetc(fin)) != EOF) {
    chars++;
    if (c == '\n') {
      lines++;
    }
    if (c == '\n' || c == ' ' || c == '\t') {
      state = WC_OUT;
    } else if (state == WC_OUT) {
      state = WC_IN;
      words++;
    }
  }

  printf(" %d %d %d\n", lines, words, chars);
}