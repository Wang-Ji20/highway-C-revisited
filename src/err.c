#include "hcr.h"

/* unix style error handling function
 *  error code stored in errno.
 */
void unix_panic(const char *msg) {
  fprintf(stderr, "[UNIX error] %s, %s\n", msg, strerror(errno));
  exit(errno);
}

/* posix style error handling function
 *  error code returned
 */
void posix_panic(int code, const char *msg) {
  fprintf(stderr, "[POSIX error] %s: %s\n", msg, strerror(code));
  exit(code);
}

/*
 * normal panic
 */
void panic(const char *msg) {
  fprintf(stderr, "[APP error] %s\n", msg);
  exit(-1);
}