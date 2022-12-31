#include "hcr.h"

int main() {
  printf("hello, world.\n");
  FILE *fd;
  if (fd = fopen("./CChecker.c", "r")) {
    CChecker(fd);
  } else {
    printf("error: %s\n", strerror(errno));
  }
  fclose(fd);
  tminfo();

  int btest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printf("%d\n", binSearch(5, btest, 10));
  return 0;
}