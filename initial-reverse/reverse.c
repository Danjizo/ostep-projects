#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Plan to follow
 *
 * If we read from a file, just use fseek, etc.
 * If we read from stdin, then we need to save all the input into a buffer (I
 * see no other way). We check and keep track of where the newlines are in the
 * buffer/file. Then using an index in case of stdin or fseek in case of a file,
 * we can print each line but going backwards in the newline-positions array.
 * */

#define LINE_BUFF_SZ 100
#define LINE_OFFSET_ARR_SZ 256

// Reads stdin into a buffer. The returned buffer must be free'd after use.
// Returns NULL if read fails.
char *read_stdin(size_t *n) {
  size_t size = 4096, count;
  char c;
  char *buffer = (char *)malloc(size);
  if (!buffer)
    return NULL;
  // Read stdin
  while ((c = fgetc(stdin)) != EOF) {
    // Get more memory!
    if (count + 1 >= size) {
      size *= 2; // Double the buffer size
      char *new_buffer = (char *)realloc(buffer, size);
      if (!new_buffer) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
    }
    buffer[count++] = (char)c;
  }
  buffer[count] = 0; // Null-terminate buffer
  if (n)             // Update n to communicate the buffer length
    *n = count;      // The buffer length returned includes the null terminator,
                     // while functions like strlen don't.
  return buffer;
}

int main(int argc, char *argv[]) {

  // Check for invalid number of arguments
  if (argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  FILE *in, *out = stdout;

  // Main structure of program

  // Write to file
  if (argc == 3) {
    // Guard against files being the same
    struct stat stat_in, stat_out;
    stat(argv[1], &stat_in); // returns 0 if success
    stat(argv[2], &stat_out);
    if (stat_in.st_dev == stat_out.st_dev &&
        stat_in.st_ino == stat_out.st_ino) {
      fprintf(stderr, "reverse: input and output file must differ\n");
      exit(EXIT_FAILURE);
    }

    out = fopen(argv[2], "w");
    if (!out) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      exit(EXIT_FAILURE);
    }
  }

  // Read from stdin
  if (argc < 2) {
    size_t stdin_buff_sz = 0;
    char *stdin_buff = read_stdin(&stdin_buff_sz);
    if (!stdin_buff) {
      fprintf(stderr, "reverse: cannot read stdin\n");
      exit(EXIT_FAILURE);
    }
    in = fmemopen(stdin_buff, stdin_buff_sz, "r");
    if (!in) {
      free(stdin_buff);
      fprintf(stderr, "reverse: cannot open stdin\n");
      exit(EXIT_FAILURE);
    }
  }

  // Read from file
  else {
    in = fopen(argv[1], "r");
    if (!in) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
    }
  }

  // Main logic goes here

  int line_offset_arr[LINE_OFFSET_ARR_SZ] = {0};
  char c, buff[LINE_BUFF_SZ];
  int line_count = 0;
  while ((c = fgetc(in)) != EOF) {
    if (c == '\n' && line_count < LINE_OFFSET_ARR_SZ)
      line_offset_arr[++line_count] =
          ftell(in); // The first entry will always be 0
  }

  for (; line_count;) {
    fseek(in, line_offset_arr[--line_count], SEEK_SET);
    fgets(buff, LINE_BUFF_SZ, in);
    fprintf(out, "%s", buff);
  }

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}
