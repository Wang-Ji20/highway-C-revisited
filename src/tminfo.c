#include "hcr.h"

/**
 * @brief This function prints out the type length infomation on target machine.
 *
 */
void tminfo() {
  printf("int       size %ld min %d max %d\n", sizeof(int), INT_MIN, INT_MAX);
  printf("long      size %ld min %ld max %ld\n", sizeof(long), LONG_MIN,
         LONG_MAX);
  printf("char      size %ld min %d max %d\n", sizeof(char), CHAR_MIN,
         CHAR_MAX);
  printf("long long size %ld min %lld max %lld\n", sizeof(long long), LLONG_MIN,
         LLONG_MAX);
}