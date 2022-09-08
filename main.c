#include "./include/SVD_Power_Method.h"
#include "./include/paillier.h"
#include "./include/randomvector.h"

#include <gmp.h>
#include <sodium.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_matrix(mpf_t **matrix, int nbDevice, int nbDataItem, char *filename) {
  size_t vector_length_str = nbDataItem * 11 - 1; // 10 bits of each + space
  char *vector_with_space = malloc(vector_length_str * sizeof(char));

  int pos = 0, dev = 0;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  FILE *dataFile = fopen(filename, "rb");

  if (!dataFile) {
    fputs("not possible to read file!\n", stderr);
    exit(1);
  }

  while ((read = getline(&line, &len, dataFile)) != -1) {
    strncpy(vector_with_space, line, vector_length_str);
    char *vector_splited = strtok(vector_with_space, " ");

    while (vector_splited != NULL) {
      mpf_set_si(matrix[dev][pos], atoi(vector_splited));
      pos += 1;
      if (pos == nbDataItem)
        break;
      vector_splited = strtok(NULL, " ");
    }
    pos = 0;
    dev += 1;
  }
  fclose(dataFile);
}

void enc_zero_one(mpz_t val_zero, mpz_t val_one, paillier_public_key *pub) {
  mpz_t pla_zero, pla_one;
  mpz_init_set_si(pla_zero, 0);
  mpz_init_set_si(pla_one, 1);
  paillier_encrypt(val_zero, pla_zero, pub);
  paillier_encrypt(val_one, pla_one, pub);
  mpz_clear(pla_zero);
  mpz_clear(pla_one);
}

int main() {

  if (sodium_init() < 0) {
    /* panic! the library couldn't be initialized, it is not safe to use */
    exit(1);
  }
  ruv_buffer_t buffer;

  // load matrix from file
  int nbDevice = 1024;
  int nbDataItem = 256;
  char *dataFile = "./data_sample/data_int_max_1024_256";
  uint32_t i, j, k = 50;

  mpf_t **matrix = (mpf_t **)malloc(nbDevice * sizeof(mpf_t *));
  for (i = 0; i < nbDevice; i++) {
    matrix[i] = (mpf_t *)malloc(nbDataItem * sizeof(mpf_t));
    for (j = 0; j < nbDataItem; j++) {
      mpf_init(matrix[i][j]);
    }
  }

  load_matrix(matrix, nbDevice, nbDataItem, dataFile);

  // encryption preparation
  paillier_public_key pub;
  paillier_private_key pri;

  paillier_public_init(&pub);
  paillier_private_init(&pri);

  paillier_keygen(&pub, &pri, 128);

  mpz_t val_zero, val_one;
  mpz_init(val_zero);
  mpz_init(val_one);
  enc_zero_one(val_zero, val_one, &pub);

  // noise preparation
  long int noise_accuracy = 1000000;
  double **noise_vector = (double **)malloc(nbDevice * sizeof(double *));
  mpz_t **noise_vector_int = (mpz_t **)malloc(nbDevice * sizeof(mpz_t *));
  int **noise_sign = (int **)malloc(nbDevice * sizeof(int *));
  mpz_t **cipher_zero_one = (mpz_t **)malloc(nbDevice * sizeof(mpz_t *));
  for (i = 0; i < nbDevice; i++) {
    noise_vector[i] = (double *)malloc(k * sizeof(double));
    noise_vector_int[i] = (mpz_t *)malloc(k * sizeof(mpz_t));
    noise_sign[i] = (int *)malloc(k * sizeof(int));
    cipher_zero_one[i] = (mpz_t *)malloc(k * sizeof(mpz_t));
    for (j = 0; j < k; j++) {
      mpz_init(noise_vector_int[i][j]);
      mpz_init(cipher_zero_one[i][j]);
      noise_sign[i][j] = 1;
    }
  }
  mpf_t noise_tmp;
  mpf_init(noise_tmp);

  // SVD preparation
  int times_p = 5, times_n = 10;
  bool isReachEp = false;
  bool isInit = true;

  mpf_t **eigenspace = (mpf_t **)malloc(nbDevice * sizeof(mpf_t *));
  mpf_t **eigenspace_prev = (mpf_t **)malloc(nbDevice * sizeof(mpf_t *));
  for (i = 0; i < nbDevice; i++) {
    eigenspace[i] = (mpf_t *)malloc(nbDataItem * sizeof(mpf_t));
    eigenspace_prev[i] = (mpf_t *)malloc(nbDataItem * sizeof(mpf_t));
    for (j = 0; j < nbDataItem; j++) {
      mpf_init(eigenspace[i][j]);
      mpf_init_set_d(eigenspace_prev[i][j], 0.);
    }
  }

  // aggregation preparation
  mpf_t *eigenspace_aggr = (mpf_t *)malloc(k * sizeof(mpf_t));
  for (i = 0; i < k; i++) {
    mpf_init(eigenspace_aggr[i]);
  }
  // start iteration
  for (int tn = 0; tn < (times_n / times_p); tn++) {

    if (isReachEp)
      break;
    for (i = 0; i < nbDevice; i++) {
      svd(eigenspace[i], isReachEp, isInit, eigenspace_prev[i], matrix[i],
          nbDataItem, times_p, k);
    }

    // add noise
    ruv_init(&buffer);
    for (i = 0; i < nbDevice; i++) {
      ruv_generate(&buffer, k, noise_vector[i]);
      for (j = 0; j < k; j++) {
        mpf_set_d(noise_tmp, noise_vector[i][j]);
        mpf_add(eigenspace[i][j], eigenspace[i][j], noise_tmp);

        // turn each noise into integer for encrption later
        if (noise_vector[i][j] < 0) {
          noise_sign[i][j] = -1;
          noise_vector[i][j] *= -1;
        }
        mpz_set_si(noise_vector_int[i][j],
                   (int)(noise_vector[i][j] * noise_accuracy));
      }
    }
    ruv_free(&buffer);

    // secure aggregation
    for (i = 0; i < k; i++) {
      for (j = 0; j < nbDevice; j++) {
        mpf_add(eigenspace_aggr[i], eigenspace_aggr[i], eigenspace[j][i]);
      }
      mpf_div_ui(eigenspace_aggr[i], eigenspace_aggr[i], nbDevice);
    }

    // server chooses random index
    uint32_t edge_index = randombytes_uniform(nbDevice);

    // encrypt zero matrix and all-one matrix
    for (i = 0; i < nbDevice; i++) {
      for (j = 0; j < k; j++) {
        if (i == edge_index) {
          paillier_redandomise(cipher_zero_one[i][j], val_zero, &pub);
        } else {
          paillier_redandomise(cipher_zero_one[i][j], val_one, &pub);
        }
        // combine ciphermatrix and noise
        paillier_homomorphic_multc(cipher_zero_one[i][j], cipher_zero_one[i][j],
                                   noise_vector_int[i][j], &pub);
      }
    }

    // // combien ciphermatrix and noise
    // for (i = 0; i < nbDevice; i++) {
    //   for (j = 0; j < k; j++) {
    // paillier_homomorphic_multc(cipher_zero_one[i][j], cipher_zero_one[i][j],
    //                            noise_vector_int[i][j], &pub);
    //   }
    // }

    // decrypt combination and cancel noise from aggegation
    for (i = 0; i < k; i++) {
      for (j = 0; j < nbDevice; j++) {
        paillier_decrypt(cipher_zero_one[j][i], cipher_zero_one[j][i], &pri);
        mpf_set_z(noise_tmp, cipher_zero_one[j][i]);
        mpf_div_ui(noise_tmp, noise_tmp, noise_accuracy);
        if (noise_sign[j][i] == -1) {
          mpf_add(eigenspace_aggr[i], eigenspace_aggr[i], noise_tmp);
        } else {
          mpf_sub(eigenspace_aggr[i], eigenspace_aggr[i], noise_tmp);
        }
      }
    }

    if (isReachEp) {
      break;
    } else {
      // copy eigenspace_aggr to eigenspace_prev
      for (i = 0; i < k; i++) {
        for (j = 0; j < nbDevice; j++) {
          mpf_set(eigenspace_prev[j][i], eigenspace_aggr[i]);
        }
        mpf_set_ui(eigenspace_aggr[i], 0);
      }
    }
  } // end of iteration


  // free matrix and eigenspace
  paillier_public_clear(&pub);
  paillier_private_clear(&pri);
  mpz_clear(val_zero);
  mpz_clear(val_one);
  mpf_clear(noise_tmp);

  for (i = 0; i < k; i++) {
    mpf_clear(eigenspace_aggr[i]);
  }

  for (i = 0; i < nbDevice; i++) {
    for (j = 0; j < nbDataItem; j++) {
      mpf_clear(matrix[i][j]);
      mpf_clear(eigenspace_prev[i][j]);
      mpf_clear(eigenspace[i][j]);
    }

    for (j = 0; j < k; j++) {
      mpz_clear(noise_vector_int[i][j]);
      mpz_clear(cipher_zero_one[i][j]);
    }
    free(noise_sign[i]);
    free(matrix[i]);
    free(eigenspace_prev[i]);
    free(eigenspace[i]);
    free(noise_vector[i]);
    free(noise_vector_int[i]);
    free(cipher_zero_one[i]);
  }
  free(noise_sign);
  free(matrix);
  free(eigenspace_prev);
  free(eigenspace);
  free(noise_vector);

  free(noise_vector_int);
  free(cipher_zero_one);
  free(eigenspace_aggr);
}
