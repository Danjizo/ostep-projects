#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    exit(EXIT_FAILURE);
  }

  FILE *in = stdin;

  if (argc > 2) {
    in = fopen(argv[2], "r");
    if (!in) {
      printf("wgrep: cannot open file\n");
      exit(EXIT_FAILURE);
    }
  }

  char *target = argv[1];

  char *line = NULL;
  size_t line_alloc_sz = 0;

  char *line_ptr, *target_ptr;

  while (getline(&line, &line_alloc_sz, in) != EOF) {
    line_ptr = line;
    target_ptr = target;
    // Linear search on the line
    for (; *line_ptr != 0 && *target_ptr != 0; line_ptr++) {
      if (*line_ptr != *target_ptr) {
        target_ptr = target; // 'Rewind' the target pointer
      } else {
        target_ptr++;
      }
    }
    if (*target_ptr == 0) // Target found in line
      fprintf(stdout, "%s", line);
  }

  free(line);
  fclose(in);

  exit(EXIT_SUCCESS);
}
