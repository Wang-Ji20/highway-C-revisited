#include "hcr.h"

int binSearch(int x, int vec[], int sz) {
  int lo, mid, hi;
  hi = sz - 1;
  lo = 0;

  while (lo <= hi) {
    mid = (lo + hi) / 2;
    if (x < vec[mid]) {
      hi = mid - 1;
    } else if (x == vec[mid]) {
      return mid;
    } else {
      lo = mid + 1;
    }
  }

  return -1;
}