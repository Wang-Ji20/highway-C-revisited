#include "hcr.h"

int main() {
  printf("hello, world.\n");
  FILE *fd;
  if (fd = fopen("./1.txt", "r")) {
    wc(fd);
  } else {
    printf("error: %s\n", strerror(errno));
  }

  return 0;
}