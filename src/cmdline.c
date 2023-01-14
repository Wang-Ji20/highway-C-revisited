/*********************************
 * read argument Helper functions *
 **********************************/

#include "hcr.h"

// read a number from terminal
size_t readarg_num(usageFunc *uf) {
  char *endptr = NULL;
  size_t n = 0;

  int olderrno = errno;
  errno = 0;

  n = strtol(optarg, NULL, 10);
  if (errno) {
    missing_args(uf);
  }

  if (*endptr != '0')
    missing_args(uf);

  errno = olderrno;
  return n;
}

// open a file from terminal
FILE *readarg_openfile(usageFunc *uf) {
  FILE *f;
  if (((f) = fopen(optarg, "r")) == NULL) {
    missing_args(uf);
  }
  return f;
}

// read a number from a string
size_t read_num(char *f, char **next, int base) {
  errno = 0;
  size_t num = 0;
  if ((num = strtoll(f, next, base)), errno) {
    perror("overflow in addr ");
  }
  return num;
}