#include <gmp.h>

#include <sodium.h>

#include "./include/SVD_Power_Method.h"

void sort_vector(mpf_t *vector, int dim) {
  mpf_t tmp;
  mpf_init(tmp);
  int isSorted;

  for (int i = 0; i < dim - 1; i++) {
    isSorted = 1;
    for (int j = 0; j < dim - 1 - i; j++) {
      if (mpf_cmp(vector[j], vector[j + 1]) < 0) {
        mpf_set(tmp, vector[j]);
        mpf_set(vector[j], vector[j + 1]);
        mpf_set(vector[j + 1], tmp);
        isSorted = 0;
      }
    }
    if (isSorted)
      break;
  }
}

void svd_0(mpf_t *eigenspace, bool isReachEp, mpf_t *eigenspace_prev, mpf_t *vector, uint32_t dim, int times_p) {


  mpf_t r1, r2, max_val, *y, tmp, ep;
  mpf_init_set_d(r1, 0.);
  mpf_init_set_d(r2, 0.);
  mpf_init_set_d(max_val, 0.);
  mpf_init(tmp);
  mpf_init_set_d(ep, 0.00000001);

  y = malloc(dim * sizeof(mpf_t));
  for (uint32_t n = 0; n < dim; n++) {
    mpf_init_set_d(y[n], 1.);
  }

  for (int t = 0; t < times_p; t++) {
    mpf_set(r2, r1);

    // only for one line vector
    for (uint32_t i = 0; i < dim; i++) {
      mpf_mul(tmp, vector[i], y[i]);
      mpf_add(eigenspace[i], eigenspace_prev[i], tmp);
    }

    mpf_div(r1, eigenspace[1], y[1]);

    for (uint32_t j = 0; j < dim; j++) {
      if (mpf_cmp(max_val, eigenspace[j]) <= 0) {
        mpf_set(max_val, eigenspace[j]);
      }
    }

    for (uint32_t l = 0; l < dim; l++) {
      mpf_div(y[l], eigenspace[l], max_val);
      mpf_set(eigenspace[l], y[l]);
    }

    mpf_sub(tmp, r2, r1);
    mpf_abs(tmp, tmp);
    if (mpf_cmp(tmp, ep) < 0) {
      isReachEp = true;
      break;
    }
  }

  sort_vector(eigenspace, dim);

  mpf_clear(r1);
  mpf_clear(r2);
  mpf_clear(max_val);
  mpf_clear(tmp);
  mpf_clear(ep);
  free(y);
}

void svd_1(mpf_t *eigenspace, bool isReachEp, mpf_t *eigenspace_prev, mpf_t *vector, uint32_t dim, int times_p, uint32_t k) {


  mpf_t r1, r2, max_val, *y, tmp, ep;
  mpf_init_set_d(r1, 0.);
  mpf_init_set_d(r2, 0.);
  mpf_init_set_d(max_val, 0.);
  mpf_init(tmp);
  mpf_init_set_d(ep, 0.00000001);

  y = malloc(dim * sizeof(mpf_t));
  for (int n = 0; n < dim; n++) {
    mpf_init_set_d(y[n], 1.);
  }

  for (int t = 0; t < times_p; t++) {
    mpf_set(r2, r1);

    // only for one line vector
    for (uint32_t i = 0; i < k; i++) {
      mpf_mul(tmp, vector[i], y[i]);
      mpf_add(eigenspace[i], eigenspace_prev[i], tmp);
    }

    mpf_div(r1, eigenspace[1], y[1]);

    for (uint32_t j = 0; j < k; j++) {
      if (mpf_cmp(max_val, eigenspace[j]) <= 0) {
        mpf_set(max_val, eigenspace[j]);
      }
    }

    for (uint32_t l = 0; l < k; l++) {
      mpf_div(y[l], eigenspace[l], max_val);
      mpf_set(eigenspace[l], y[l]);
    }

    mpf_sub(tmp, r2, r1);
    mpf_abs(tmp, tmp);
    if (mpf_cmp(tmp, ep) < 0) {
      isReachEp = true;
      break;
    }
  }

  sort_vector(eigenspace, k);

  mpf_clear(r1);
  mpf_clear(r2);
  mpf_clear(max_val);
  mpf_clear(tmp);
  mpf_clear(ep);
  free(y);
}

void svd(mpf_t *eigenspace, bool isReachEp, bool isInit, mpf_t *eigenspace_prev, mpf_t *vector, uint32_t dim, int times_p, uint32_t k) {


  mpf_t r1, r2, max_val, *y, tmp, ep;
  mpf_init_set_d(r1, 0.);
  mpf_init_set_d(r2, 0.);
  mpf_init_set_d(max_val, 0.);
  mpf_init(tmp);
  mpf_init_set_d(ep, 0.00000001);
  uint32_t keepLength;

  if (isInit){
keepLength = dim;
  }
  else{
    keepLength = k;
  }

  y = malloc(dim * sizeof(mpf_t));
  for (int n = 0; n < keepLength; n++) {
    mpf_init_set_d(y[n], 1.);
  }

  for (int t = 0; t < times_p; t++) {
    mpf_set(r2, r1);

    // only for one line vector
    for (uint32_t i = 0; i < keepLength; i++) {
      mpf_mul(tmp, vector[i], y[i]);
      mpf_add(eigenspace[i], eigenspace_prev[i], tmp);
    }

    mpf_div(r1, eigenspace[1], y[1]);

    for (uint32_t j = 0; j < keepLength; j++) {
      if (mpf_cmp(max_val, eigenspace[j]) <= 0) {
        mpf_set(max_val, eigenspace[j]);
      }
    }

    for (uint32_t l = 0; l < keepLength; l++) {
      mpf_div(y[l], eigenspace[l], max_val);
      mpf_set(eigenspace[l], y[l]);
    }

    mpf_sub(tmp, r2, r1);
    mpf_abs(tmp, tmp);
    if (mpf_cmp(tmp, ep) < 0) {
      isReachEp = true;
      break;
    }
  }

  sort_vector(eigenspace, keepLength);

  isInit = false;
  mpf_clear(r1);
  mpf_clear(r2);
  mpf_clear(max_val);
  mpf_clear(tmp);
  mpf_clear(ep);
  free(y);
}

// void svd(mpf_t *eigenspace, bool isReachEp, mpf_t *eigenspace_prev, mpf_t *vector, int dim, int times_p, int k) {


//   mpf_t r1, r2, max_val, *y, tmp, ep;
//   mpf_init_set_d(r1, 0.);
//   mpf_init_set_d(r2, 0.);
//   mpf_init_set_d(max_val, 0.);
//   mpf_init(tmp);
//   mpf_init_set_d(ep, 0.00000001);

//   y = malloc(dim * sizeof(mpf_t));
//   for (int n = 0; n < dim; n++) {
//     mpf_init_set_d(y[n], 1.);
//   }

//   for (int t = 0; t < times_p; t++) {
//     mpf_set(r2, r1);

//     // only for one line vector
//     for (int i = 0; i < dim; i++) {
//       mpf_mul(tmp, vector[i], y[i]);
//       mpf_add(eigenspace[i], eigenspace_prev[i], tmp);
//     }

//     mpf_div(r1, eigenspace[1], y[1]);

//     for (int j = 0; j < dim; j++) {
//       if (mpf_cmp(max_val, eigenspace[j]) <= 0) {
//         mpf_set(max_val, eigenspace[j]);
//       }
//     }

//     for (int l = 0; l < dim; l++) {
//       mpf_div(y[l], eigenspace[l], max_val);
//       mpf_set(eigenspace[l], y[l]);
//     }

//     mpf_sub(tmp, r2, r1);
//     mpf_abs(tmp, tmp);
//     if (mpf_cmp(tmp, ep) < 0) {
//       isReachEp = true;
//       break;
//     }
//   }

//   sort_vector(eigenspace, dim);

//   mpf_clear(r1);
//   mpf_clear(r2);
//   mpf_clear(max_val);
//   mpf_clear(tmp);
//   mpf_clear(ep);
//   free(y);
// }

// int main() {
//   if (sodium_init() < 0) {
//     /* panic! the library couldn't be initialized, it is not safe to use */
//     exit(1);
//   }
//   mpf_set_default_prec(128);
//   mpf_t *vector, *eigenspace;
//   int dim = 10, k = 7, times_p = 100;
//   vector = malloc(dim * sizeof(mpz_t));
//   eigenspace = malloc(dim * sizeof(mpf_t));
//   for (int n = 0; n < dim; n++) {
//     mpf_init_set_ui(vector[n], randombytes_uniform(10000));
//     mpf_init(eigenspace[n]);
//   }

//   svd(eigenspace, vector, dim, times_p, k);

//   for (int i = 0; i < k; i++) {
//     gmp_printf("%.*Ff \n", 30, eigenspace[i]);
//     mpf_clear(eigenspace[i]);
//     mpf_clear(vector[i]);
//   }

//   free(vector);
//   free(eigenspace);

//   return 0;
// }
