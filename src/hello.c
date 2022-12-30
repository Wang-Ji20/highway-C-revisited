#include "hcr.h"

int main() {
  printf("hello, world.\n");
  FILE *fd;
  if (fd = fopen("./CChecker.c", "r")) {
    CChecker(fd);
  } else {
    printf("error: %s\n", strerror(errno));
  }

  return 0;
}