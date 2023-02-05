#include "hcr.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *g_progName;
static size_t g_curTimestamp;
static size_t g_setBits, g_asso, g_blockBits, g_verbose, g_debug;
static size_t g_hits, g_misses, g_evictions;
enum access_status { MISS = 0x1, HIT = 0x10, EVICTION = 0x100 };

/* cache entry */
typedef struct {
  char valid;
  size_t cacheTag;  /* address */
  size_t timestamp; /* for LRU */
} cache_entry;

cache_entry **g_cacheTable; /* lazy allocation */

typedef struct {
  char type;
  size_t addr;
  size_t size;
} trace_t;

/* shows usage information */

static void usage() {
  fputs("Usage: csim [-hv] -s <num> -E <num> -b <num> -t <file>\n"
        "Options:\n"
        "  -h         Print this help message.\n"
        "  -v         Optional verbose flag.\n"
        "  -s <num>   Number of set index bits.\n"
        "  -E <num>   Number of lines per set.\n"
        "  -b <num>   Number of block offset bits.\n"
        "  -t <file>  Trace file.\n"
        "\n"
        "Examples:\n"
        "  linux>  csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
        "  linux>  csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n",
        stderr);
}

/***********
  utilities
************/

// for debug little utility
inline static void debug(const char *s) {
  if (g_debug)
    fprintf(stderr, "[DEBUG] %d %s", __LINE__, s);
}

inline static void verbose(trace_t *t, int flags) {
  if (g_verbose) {
    printf("%c %lx,%ld", t->type, t->addr, t->size);
    if (flags & MISS)
      printf(" miss");
    if (flags & EVICTION)
      printf(" eviction");
    if (flags & HIT)
      printf(" hit");
    printf("\n");
  }
}

// get the B area of a cache addr
inline static size_t getB(size_t addr) {
  return (addr << (__WORDSIZE - g_blockBits)) >> (__WORDSIZE - g_blockBits);
}

// get the Tag area of a cache addr
inline static size_t getTag(size_t addr) {
  return (addr >> (g_blockBits + g_setBits));
}

// get the set area of a cache addr
inline static size_t getSet(size_t addr) {
  size_t t = __WORDSIZE - g_setBits - g_blockBits;
  return (addr << t) >> (t + g_blockBits);
}

// get max
inline int max(size_t a, size_t b) { return (a > b) ? a : b; }

/**************************
   cache table functions
***************************/

// init cache table, no return
void init_ctable() {
  size_t sets = pow(2, g_setBits);
  g_cacheTable = calloc(sets, __WORDSIZE);
}

// free cache table
void free_ctable() {
  size_t sets = pow(2, g_setBits);
  for (size_t i = 0; i < sets; i++) {
    free(g_cacheTable[i]);
  }
  free(g_cacheTable);
}

// wrapper function for calloc
void *Calloc(size_t ele_num, size_t ele_size) {
  void *retptr;
  if ((retptr = calloc(ele_num, ele_size)) == NULL)
    missing_args(usage);
  return retptr;
}

// setup a centry
cache_entry setup_centry(size_t tag) {
  cache_entry ce;
  ce.valid = 1;
  ce.cacheTag = tag;
  ce.timestamp = g_curTimestamp++;
  return ce;
}

int lru(size_t group) {
  size_t ts = __SIZE_MAX__;
  int idx = 0;
  for (size_t i = 0; i < g_asso; i++) {
    if (g_cacheTable[group][i].timestamp < ts) {
      idx = i;
      ts = g_cacheTable[group][i].timestamp;
    }
  }
  return idx;
}

/**
 * @brief search the cache table, insert when not exist
 *
 * @param t the trace entry, our target
 * @return HIT if hit, MISS if miss
 */
int find_ctable(trace_t *t) {
  size_t group = getSet(t->addr);
  size_t tag = getTag(t->addr);

  if (g_cacheTable[group] == NULL)
    g_cacheTable[group] = Calloc(g_asso, sizeof(cache_entry));

  int insert_point = -1;
  for (size_t i = 0; i < g_asso; i++) {
    if (g_cacheTable[group][i].valid &&
        g_cacheTable[group][i].cacheTag == tag) {
      g_cacheTable[group][i].timestamp = g_curTimestamp++;
      g_hits++;
      return HIT;
    }
    if (!g_cacheTable[group][i].valid && (insert_point == -1))
      insert_point = i;
  }

  if (insert_point > -1) {
    g_cacheTable[group][insert_point] = setup_centry(tag);
    g_misses++;
    return MISS;
  }

  /* LRU */
  insert_point = lru(group);
  g_cacheTable[group][insert_point] = setup_centry(tag);

  g_misses++;
  g_evictions++;
  return MISS | EVICTION;
}

/************************************************
  read and process trace file routines.
*************************************************/

// read a single entry, type if success; 0 bad line; -1 EOF
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
      return -1;
    }
  } else {
    fprintf(stderr, "bad instruction type\n");
    return -1;
  }
  peek += 2;

  char *next = NULL;
  trace->addr = read_num(buffer + peek, &next, 16);
  trace->size = read_num(++next, NULL, 10);

  free(buffer);

  return trace->type;
}

// print one entry
inline static void print_entry(const trace_t *t) {
  if (g_debug)
    printf("%c %lx,%ld\n", t->type, t->addr, t->size);
}

// print one entry, in much detail
inline static void print_entryd(const trace_t *t) {
  debug("print entry\n");
  if (g_debug)
    printf("%lx %lx %lx %lx\n", t->addr, getTag(t->addr), getSet(t->addr),
           getB(t->addr));
}

// read all trace entries, and prints them out
void extract_entries(FILE *f) {
  trace_t trace;
  while (read_trace_entry(f, &trace) > -1) {
    if (trace.type == 'I')
      continue;
    print_entry(&trace);
    print_entryd(&trace);
    int flags = find_ctable(&trace);
    if (trace.type == 'M')
      flags |= find_ctable(&trace);
    verbose(&trace, flags);
  }
  return;
}

void printSummary(size_t hits, size_t misses, size_t evictions) {
  printf("hits:%zu misses:%zu evictions:%zu\n", hits, misses, evictions);
}

// main function
void cmd_csim(int argc, char *argv[]) {
  size_t opt;
  FILE *traceFile;

  g_verbose = g_asso = g_blockBits = g_curTimestamp = g_setBits = g_debug = 0;

  g_progName = argv[0];

  while ((opt = getopt(argc, argv, "hdvs:E:b:t:")) != -1) {
    switch (opt) {
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
      break;

    case 's':
      g_setBits = readarg_num();
      break;

    case 'E':
      g_asso = readarg_num();
      break;

    case 'b':
      g_blockBits = readarg_num();
      break;

    case 't':
      traceFile = readarg_openfile();
      break;

    case 'v':
      g_verbose = 1;
      break;

    case 'd':
      g_debug = 1;
      break;

    default:
      invalid_option(opt, usage);
      break;
    }
  }

  if (!g_setBits || !g_asso || !g_blockBits || !traceFile) {
    missing_args(usage);
  }

  init_ctable();

  extract_entries(traceFile);

  free_ctable();

  Fclose(traceFile);

  printSummary(g_hits, g_misses, g_evictions);

  exit(EXIT_SUCCESS);
}
