#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// wc program
void wc(FILE *fin);
#define WC_IN 0
#define WC_OUT 1

// c checker program
void CChecker(FILE *fin);
#define CCK_FILE 0
#define CCK_SLASH 1
#define CCK_STAR 2
#define CCK_COMMENT 3
#define CCK_BLOCKCMT 4
#define CCK_BACKSLASH 5

// error handling
void unix_panic(const char* msg);
void posix_panic(int code, const char* msg);
void panic(const char* msg);

// driver
