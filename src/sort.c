#include "hcr.h"

void shellsort(int v[], int n) {
  int gap, i, j;

  for (gap = n / 2; gap > 0; gap /= 2)
    for (i = gap; i < n; i++)
      for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap) {
        swap(v, j, j + gap);
      }
}

void quicksort(int v[], int n) {
  int i, last;

  if (n <= 1)
    return;
  swap(v, 0, rand() % n);
  last = 0;
  for (i = 1; i < n; i++)
    if (v[i] < v[0])
      swap(v, ++last, i);
  swap(v, 0, last);
  quicksort(v, last);
  quicksort(v + last + 1, n - last - 1);
}
