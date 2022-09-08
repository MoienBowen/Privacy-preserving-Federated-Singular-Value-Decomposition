#include "../include/paillier.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  paillier_public_key pub;
  paillier_private_key pri;

  paillier_public_init(&pub);
  paillier_private_init(&pri);

  paillier_keygen(&pub, &pri, 128);

  //////////////

  mpz_t plaintext, ciphertext, r, n2;
  mpz_init(plaintext);
  mpz_init(ciphertext);
  mpz_init_set_si(r, 5);
  mpz_init(n2);

  mpz_set_si(plaintext, 1);
  paillier_encrypt(ciphertext, plaintext, &pub);

  paillier_redandomise(ciphertext, ciphertext, &pub);



  mpf_t noise_sample, noise_tmp;
  mpf_init(noise_tmp);
  mpf_init_set_d(noise_sample, 0.6754356789);
  mpf_mul_ui(noise_tmp, noise_sample, 100000);
  // mpf_clear(noise_sample);

  mpz_t noise_int;
  mpz_init(noise_int);
  mpz_set_f(noise_int, noise_tmp);

  paillier_homomorphic_multc(ciphertext, ciphertext, noise_int, &pub);
  // mpz_clear(noise_int);

  mpz_t dec_noise_sample_int;
  mpz_init(dec_noise_sample_int);

  paillier_decrypt(dec_noise_sample_int, ciphertext, &pri);
  gmp_printf("%Zd\n", dec_noise_sample_int);

  // mpz_clear(plaintext);
  // mpz_clear(ciphertext);
  // mpz_clear(r);
  // mpz_clear(n2);
  return 0;
}
