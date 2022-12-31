#include "hcr.h"

void shellsort(int v[], int n) {
  int gap, i, j, temp;

  for (gap = n / 2; gap > 0; gap /= 2)
    for (i = gap; i < n; i++)
      for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap) {
        swap(v, j, j + gap);
      }
}
