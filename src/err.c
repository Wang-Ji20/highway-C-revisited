#include "hcr.h"

/* unix style error handling function
 *  error code stored in errno.
 */
_Noreturn void unix_panic(const char *msg) {
  fprintf(stderr, "[FATAL UNIX error] %s, %s\n", msg, strerror(errno));
  exit(EXIT_FAILURE);
}

/* posix style error handling function
 *  error code returned
 */
_Noreturn void posix_panic(int code, const char *msg) {
  fprintf(stderr, "[FATAL POSIX error] %s: %s\n", msg, strerror(code));
  exit(EXIT_FAILURE);
}

/*
 * normal panic
 */
_Noreturn void panic(const char *msg) {
  fprintf(stderr, "[FATAL APP error] %s\n", msg);
  fflush(stderr);
  exit(EXIT_FAILURE);
}

void fpanic(const char *format, ...) {
  va_list argList;
  va_start(argList, format);
  vfprintf(stderr, format, argList);
  va_end(argList);
  fflush(stderr);
  panic("");
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
 *   command line error
 */

// prints "invalid option" and quits, never return
void invalid_option(char opt, usageFunc *uf) {
  uf();
  fpanic("invalid option -- '%c'", opt);
}

// prints "missing args" and quits, never return
void missing_args(usageFunc *uf) {
  uf();
  panic("Missing required command line argument");
}

/*
 *   error recovery functions
 */
