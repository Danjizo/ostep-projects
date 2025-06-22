#include <endian.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

union u16_bytes_u {
  uint16_t number;
  uint8_t bytes[sizeof(uint16_t)];
};

union u32_bytes_u {
  uint32_t number;
  uint8_t bytes[sizeof(uint32_t)];
};

bool is_little_endian(void) {
  union u16_bytes_u x = {.number = 0x2B1A};
  return x.bytes[0] == 0x1A;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stdout, "wunzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  union u32_bytes_u count;
  uint8_t c, buffer[5] = {0};
  size_t bytes_read = 0;
  FILE *fp = NULL;
  bool little_endian = is_little_endian();

  for (int file_idx = 1; file_idx < argc; file_idx++) {
    // Open file
    fp = fopen(argv[file_idx], "r");
    if (!fp) {
      printf("wunzip: cannot open file\n");
      exit(EXIT_FAILURE);
    }

    // Read file
    while ((bytes_read = fread(&buffer, sizeof(buffer), 1, fp)) > 0) {
      // Extract the char
      c = buffer[4];

      // Extract the count
      if (little_endian) {
        count.bytes[0] = buffer[0];
        count.bytes[1] = buffer[1];
        count.bytes[2] = buffer[2];
        count.bytes[3] = buffer[3];
      } else {
        count.bytes[0] = buffer[3];
        count.bytes[1] = buffer[2];
        count.bytes[2] = buffer[1];
        count.bytes[3] = buffer[0];
      }

      // Write chars
      while (count.number-- > 0)
        fputc(c, stdout);
    }

    // Close file
    fclose(fp);
  }

  exit(EXIT_SUCCESS);
}
