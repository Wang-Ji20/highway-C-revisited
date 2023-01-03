#include "hcr.h"

static char *prog_name;

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
  usage();
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit(EXIT_FAILURE);
}

void invalid_option(char opt) {
  char invalid_msg[40];
  sprintf(invalid_msg, "invalid option -- '%c'", opt);
  csim_error(invalid_msg);
}

void missing_args() { csim_error("Missing required command line argument"); }

void readarg_num(int *n) {
  errno = 0;
  *n = strtol(optarg, NULL, 10);
  if (errno) {
    missing_args();
  }
}

void readarg_file(FILE **f) {
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

int csim(int argc, char *argv[]) {
  int opt, verbose, setIndexBits, Asso, blockBits;
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
      readarg_file(&traceFile);
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

  printf("s = %d, E = %d, b = %d, f = %c\n", setIndexBits, Asso, blockBits, fgetc(traceFile));

  Fclose(traceFile);

  return 0;
}
