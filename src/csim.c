#define _POSIX_C_SOURCE 200809L
#include "cachelab.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *prog_name;

/* only contains tag */
typedef __uint64_t cache_entry;

cache_entry **cache_table;

void usage() {
  fprintf(stderr,
          "Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n"
          "Options:\n"
          "  -h         Print this help message.\n"
          "  -v         Optional verbose flag.\n"
          "  -s <num>   Number of set index bits.\n"
          "  -E <num>   Number of lines per set.\n"
          "  -b <num>   Number of block offset bits.\n"
          "  -t <file>  Trace file.\n"
          "\n"
          "Examples:\n"
          "  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n"
          "  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n",
          prog_name, prog_name, prog_name);
}

void csim_error(char msg[]) {
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  usage();
  exit(EXIT_FAILURE);
}

void invalid_option(char opt) {
  char invalid_msg[40];
  sprintf(invalid_msg, "invalid option -- '%c'", opt);
  csim_error(invalid_msg);
}

void missing_args() { csim_error("Missing required command line argument"); }

/*
  utilities
*/
inline void debug() { fprintf(stderr, "DEBUG %d", __LINE__); }

inline __uint64_t getB(size_t b, __uint64_t addr) {
  return (addr << (__WORDSIZE - b)) >> (__WORDSIZE - b);
}

inline __uint64_t getMark(size_t g, size_t b, __uint64_t addr) {
  size_t t = 64 - b - g;
  return (addr >> (__WORDSIZE - t));
}

inline __uint64_t getGroup(size_t g, size_t b, __uint64_t addr) {
  size_t t = 64 - b - g;
  return (addr << t) >> (t + b);
}

inline int max(size_t a, size_t b) { return (a > b) ? a : b; }

/*
  read argument Helper functions
*/

void readarg_num(size_t *n) {
  errno = 0;
  *n = strtol(optarg, NULL, 10);
  if (errno) {
    missing_args();
  }
}

void readarg_openfile(FILE **f) {
  if (((*f) = fopen(optarg, "r")) == NULL) {
    missing_args();
  }
}

void Fclose(FILE *f) {
  if (fclose(f) == EOF) {
    perror("Fclose ");
    exit(EXIT_FAILURE);
  }
}

__uint64_t read_addr(char *f, char **next, int base) {
  errno = 0;
  __uint64_t num = 0;
  if ((num = strtoll(f, next, base)), errno) {
    perror("overflow in addr ");
  }
  return num;
}

/*
  read trace file routines.
*/

typedef struct {
  char type;
  __uint64_t addr;
  __uint64_t size;
} trace_t;

/*
  read a single entry, type if success 0 bad line -1 EOF
*/

int read_trace_entry(FILE *f, trace_t *trace) {
  char *buffer = NULL;
  size_t linenum = 0;
  int peek = 0;

  if (getline(&buffer, &linenum, f) < 0) {
    /* EOF or error */
    free(buffer);
    if (errno)
      perror("cannot read trace entry");
    return -1;
  }
  if (buffer[peek] == 'I') {
    trace->type = 'I';
  } else if (buffer[peek++] == ' ') {
    switch (buffer[peek]) {

    case 'M':
    case 'L':
    case 'S':
      trace->type = buffer[peek];
      break;

    default:
      fprintf(stderr, "bad instruction type\n");
      return 0;
    }
  } else {
    fprintf(stderr, "bad instruction type\n");
    return 0;
  }
  peek += 2;

  char *next = NULL;
  trace->addr = read_addr(buffer + peek, &next, 16);
  trace->size = read_addr(++next, NULL, 10);

  free(buffer);

  return trace->type;
}

/*
  print one entry
*/

void print_entry(trace_t *t) {
  printf("%c %lx,%ld\n", t->type, t->addr, t->size);
}

/*
  read all trace entries, and prints them out
*/

void extract_entries(FILE *f) {
  trace_t trace;
  while (read_trace_entry(f, &trace) > -1) {
    if (trace.type == 'I')
      continue;
    print_entry(&trace);
  }
  return;
}

void init_ctable(size_t setbits) {
  setbits = pow(2, setbits);
  cache_table = calloc(setbits, __WORDSIZE);
}

void free_ctable(size_t setbits) {
  setbits = pow(2, setbits);
  for (size_t i = 0; i < setbits; i++) {
    free(cache_table[i]);
  }
  free(cache_table);
}

int find_ctable(trace_t *t, size_t set, size_t bsize, size_t asso) {
  size_t group = getGroup(set, bsize, t->addr);
  size_t tag = getMark(set, bsize, t->addr);
  if (cache_table[group] == NULL)
    cache_table[group] = calloc(asso, sizeof(cache_entry));

  size_t i = 0;
  for (; i < asso; i++) {
    if (cache_table[group][i] == tag) {
      return 'H';
    }
    if (cache_table[group][i] == NULL)
      break;
  }

  if (i != asso) {
    cache_table[group][i] = tag;
    return 'M';
  }

  /* LRU */
  // TODO

  return 'M';
}

int main(int argc, char *argv[]) {
  size_t opt, verbose, setIndexBits, Asso, blockBits;
  FILE *traceFile;

  opt = verbose = setIndexBits = Asso = blockBits = 0;
  prog_name = argv[0];

  while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (opt) {
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
      break;

    case 's':
      readarg_num(&setIndexBits);
      break;

    case 'E':
      readarg_num(&Asso);
      break;

    case 'b':
      readarg_num(&blockBits);
      break;

    case 't':
      readarg_openfile(&traceFile);
      break;

    case 'v':
      verbose = 1;
      break;

    default:
      invalid_option(opt);
      break;
    }
  }

  if (!setIndexBits || !Asso || !blockBits || !traceFile) {
    missing_args();
  }

  init_ctable(setIndexBits);

  extract_entries(traceFile);

  free_ctable(setIndexBits);

  Fclose(traceFile);

  return 0;
}
