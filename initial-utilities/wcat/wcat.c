#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  size_t bufflen = 1024;
  char *buff = (char *)malloc(bufflen);
  // Iterate over all specified files
  for (int file_idx = 1; file_idx < argc; file_idx++) {
    FILE *fp = fopen(argv[file_idx], "r");
    if (!fp) {
      fprintf(stdout, "wcat: cannot open file\n");
      exit(EXIT_FAILURE);
    }
    while (fgets(buff, bufflen, fp)) {
      fprintf(stdout, "%s", buff);
    }
    fclose(fp);
  }
  free(buff);
  exit(EXIT_SUCCESS);
}
