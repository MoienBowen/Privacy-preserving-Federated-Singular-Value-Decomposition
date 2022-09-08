#include <assert.h>

#include <stdio.h>

#include <stdlib.h>

#include <sodium.h>


int main(void) {

  int nbDevice = 1024;
  int nbDataItem = 256;

  FILE * dataFile = fopen("./data_int_max_1024_256", "wb");

  if (!dataFile) {
    fputs("not possible to write file!\n", stderr);
    exit(1);
  }

  if (sodium_init() < 0) {
    /* panic! the library couldn't be initialized, it is not safe to use */
    exit(1);
  }

  for (int i = 0; i < nbDevice; i++) {
    for (int j = 0; j < nbDataItem; j++) {
      fprintf(dataFile, "%u ", randombytes_uniform(RAND_MAX));
    }
    fprintf(dataFile, "\n");
  }

  fclose(dataFile);
}
