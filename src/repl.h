#define _XOPEN_SOURCE 700
#include <signal.h>
#include <sys/wait.h>

#include "hcr.h"

extern const size_t MAXLINE;
extern const size_t MAXARGS;

typedef struct {
  char *cBuffer;
  size_t sztBufferCap;
  ssize_t ssztBufferSize;
} CharBuffer;

typedef void handler_t(int);

handler_t *Signal(int signum, handler_t *handler);

int parseline(char *cmdline, char **argv, int *argc);
commonFunc *sfindFunc(const char *name);
