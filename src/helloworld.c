#include "hcr.h"

int hello() {
  printf("hello, world\n");
  return 0;
}

void cmd_hello(int argc, char **argv) {
  fdebug("call command hello");
  hello();
  exit(hello());
}