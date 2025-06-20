#include <stdio.h>
#include <stdlib.h>

struct encoded_unit {
  unsigned int count;
  char c;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stdout, "wunzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  struct encoded_unit encoded_unit = {0, 0};
  int files_left = argc - 1, file_idx = 1;

  // Open the first file
  FILE *fp = fopen(argv[file_idx++], "r");
  if (!fp) {
    printf("wunzip: cannot open file\n");
    exit(EXIT_FAILURE);
  }
  files_left--;

  while (!feof(fp) || files_left > 0) {
    // Catch a possible EOF: open the next file
    if (feof(fp)) {
      if (fp) // Close the previous file (if any)
        fclose(fp);
      fp = fopen(argv[file_idx++], "r");
      if (!fp) {
        printf("wunzip: cannot open file\n");
        exit(EXIT_FAILURE);
      }
      files_left--;
    }
    fread(&encoded_unit, sizeof(struct encoded_unit), 1, fp);
    for (unsigned int i = 0; i < encoded_unit.count; i++) {
      fprintf(stdout, "%c", encoded_unit.c);
    }
  };

  fclose(fp); // Close last file opened

  exit(EXIT_SUCCESS);
}
