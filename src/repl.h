#define _XOPEN_SOURCE 700
#include <signal.h>

#include "hcr.h"

typedef struct {
  char *cBuffer;
  size_t sztBufferSize;
  ssize_t ssztBufferSize;
} CharBuffer;

typedef void handler_t(int);

handler_t *Signal(int signum, handler_t *handler);
