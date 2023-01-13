#include "hcr.h"

/* unix style error handling function
 *  error code stored in errno.
 */
void unix_panic(const char *msg) {
  fprintf(stderr, "[FATAL UNIX error] %s, %s\n", msg, strerror(errno));
  exit(errno);
}

/* posix style error handling function
 *  error code returned
 */
void posix_panic(int code, const char *msg) {
  fprintf(stderr, "[FATAL POSIX error] %s: %s\n", msg, strerror(code));
  exit(code);
}

/*
 * normal panic
 */
void panic(const char *msg) {
  fprintf(stderr, "[FATAL APP error] %s\n", msg);
  exit(-1);
}

/*
 *   warnings
 */

/* unix style error handling function
 *  error code stored in errno.
 */
void unix_warning(const char *msg) {
  fprintf(stderr, "[UNIX warning] %s, %s\n", msg, strerror(errno));
}

/* posix style error handling function
 *  error code returned
 */
void posix_warning(int code, const char *msg) {
  fprintf(stderr, "[POSIX warning] %s: %s\n", msg, strerror(code));
}

/*
 * normal warning
 */
void warning(const char *msg) { fprintf(stderr, "[APP warning] %s\n", msg); }

/*
 *   error recovery functions
 */
