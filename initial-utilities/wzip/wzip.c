#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stdout, "wzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  char target = 0, c = EOF;
  unsigned int counter = 0;
  int files_left = argc - 1, file_idx = 1;
  FILE *fp = NULL;

  do {

    // Catch a possible EOF: open the next file and get the first character
    if (c == EOF) {
      if (fp) // Close the previous file (if any)
        fclose(fp);
      fp = fopen(argv[file_idx++], "r");
      if (!fp) {
        printf("wzip: cannot open file\n");
        exit(EXIT_FAILURE);
      }
      files_left--;
      c = fgetc(fp);
    }

    if (target != c && target != 0) {
      fwrite(&counter, sizeof(unsigned int), 1,
             stdout);                           // Write the occurrences
      fwrite(&target, sizeof(char), 1, stdout); // Write the char
      counter = 1;                              // Reset the counter
    } else {
      counter++;
    }
    target = c;
  } while ((c = fgetc(fp)) != EOF || files_left > 0);

  // Write the last char encoding
  fwrite(&counter, sizeof(unsigned int), 1,
         stdout);                           // Write the occurrences
  fwrite(&target, sizeof(char), 1, stdout); // Write the char

  fclose(fp); // Close last file opened

  exit(EXIT_SUCCESS);
}
