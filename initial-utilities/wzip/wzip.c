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
    fprintf(stdout, "wzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  int8_t target = 0, c = EOF;
  union u32_bytes_u count = {.number = 0};
  FILE *fp = NULL;
  bool little_endian = is_little_endian();

  for (int file_idx = 1; file_idx < argc; file_idx++) {

    // Open file
    fp = fopen(argv[file_idx], "r");
    if (!fp) {
      printf("wzip: cannot open file\n");
      exit(EXIT_FAILURE);
    }

    // Read file
    while ((c = fgetc(fp)) != EOF) {
      if (target == c || target == 0) {
        count.number++;
      } else {

        // We stick to little endian as the standard way of encoding the count
        if (!little_endian) {
          // Swap bytes 3 and 0
          count.bytes[0] = count.bytes[3] ^ count.bytes[0]; // B3 xor B0
          count.bytes[3] = count.bytes[3] ^ count.bytes[0]; // B0
          count.bytes[0] = count.bytes[3] ^ count.bytes[0]; // B3
          // Swap bytes 2 and 1
          count.bytes[1] = count.bytes[2] ^ count.bytes[1]; // B2 xor B1
          count.bytes[2] = count.bytes[2] ^ count.bytes[1]; // B1
          count.bytes[1] = count.bytes[2] ^ count.bytes[1]; // B2
        }

        // flush the count and the char
        fwrite(&count, sizeof(count), 1,
               stdout);                             // Write the occurrences
        fwrite(&target, sizeof(target), 1, stdout); // Write the char
        count.number = 1;                           // Reset the count
      }

      // Update the target char
      target = c;
    }

    // Close file
    fclose(fp);
  }

  // flush the last char read
  fwrite(&count, sizeof(count), 1, stdout);   // Write the occurrences
  fwrite(&target, sizeof(target), 1, stdout); // Write the char

  exit(EXIT_SUCCESS);
}
