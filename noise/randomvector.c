#include "./../include/randomvector.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static const double TWO_PI = 6.283185307179586476925287;

#define BUFFER_LENGTH 1024

static const char *RandomFileName = "/dev/urandom";

void ruv_init(ruv_buffer_t *buffer) {
  memset(buffer, 0, sizeof(ruv_buffer_t));

  buffer->file = fopen(RandomFileName, "rb");
  if (buffer->file == NULL) {
    fputs("ruv_init: cannot open random device\n", stderr);
    exit(1);
  }

  buffer->array = calloc(BUFFER_LENGTH, sizeof(buffer->array[0]));
  if (buffer->array == NULL) {
    fputs("ruv_init: cannot allocate memory\n", stderr);
    exit(1);
  }

  /* Cause first call to ruv_uniform() to trigger a read. */
  buffer->offset = BUFFER_LENGTH;
}

void ruv_uniform(ruv_buffer_t *buffer, double *x) {
  /* Keep reading until the resulting random number is not zero. */
  do {
    if (buffer->offset == BUFFER_LENGTH) {
      /* We need to load a new batch of random 64-bit unsigned integers. */
      size_t nread = fread(buffer->array, sizeof(buffer->array[0]),
                           BUFFER_LENGTH, buffer->file);
      if (nread != BUFFER_LENGTH) {
        fputs("ruv_uniform: Error reading from device \n", stderr);
        exit(1);
      }
      buffer->offset = 0;
    }

    *x = (double)buffer->array[buffer->offset++] / (double)0xffffffffffffffffLU;
  } while (*x == 0.0);

  /* Now *x contains a uniformly-distributed random value in the half-open range
   * (0, 1]. */
}

void ruv_generate(ruv_buffer_t *buffer, int ndimensions, double vector[]) {
  int i;
  double A, B; /* uniform random variables */
  double radius, angle;
  double sum;

  do {
    sum = 0.0;
    for (i = 0; i < ndimensions; i += 2) {
      ruv_uniform(buffer, &A);
      ruv_uniform(buffer, &B);
      radius = sqrt(-2 * log(A));
      angle = TWO_PI * B;
      vector[i] = radius * cos(angle);
      sum += vector[i] * vector[i];
      if (i + 1 < ndimensions) {
        vector[i + 1] = radius * sin(angle);
        sum += vector[i + 1] * vector[i + 1];
      }
    }
  } while (sum == 0.0); /* While extremely unlikely, it is possible to pick a
                           zero vector. AVOID! */

  /* Convert to a unit vector by dividing through by the length. */
  sum = sqrt(sum);
  for (i = 0; i < ndimensions; ++i)
    vector[i] /= sum;
}

void ruv_free(ruv_buffer_t *buffer) {
  if (buffer->file != NULL) {
    fclose(buffer->file);
    buffer->file = NULL;
  }

  if (buffer->array != NULL) {
    free(buffer->array);
    buffer->array = NULL;
  }
}

#ifdef __cplusplus
}
#endif
