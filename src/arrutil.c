#include "hcr.h"

void swap(int v[], int i, int j) {
  int temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}