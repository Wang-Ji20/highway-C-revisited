#include "hcr.h"

// close a file
void Fclose(FILE *f) {
  if (fclose(f) == EOF) {
    perror("Fclose ");
    exit(EXIT_FAILURE);
  }
}

pid_t Fork(void) {
  pid_t pid;
  if ((pid = fork()) < 0)
    unix_panic("fork error");
  return pid;
}

void Exec(const char *path, char *const *argv, char *const *envp) {
  execve(path, argv, envp);
  unix_panic("Exec failed.");
}