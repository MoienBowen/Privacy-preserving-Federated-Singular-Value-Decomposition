#include "../include/paillier.h"
#include "tools.h"

void paillier_public_init(paillier_public_key *pub) { mpz_init(pub->n); }

void paillier_private_init(paillier_private_key *priv) {
  mpz_init(priv->lambda);
  mpz_init(priv->mu);
  mpz_init(priv->p2);
  mpz_init(priv->q2);
  mpz_init(priv->p2invq2);
  mpz_init(priv->ninv);
  mpz_init(priv->n);
}

void paillier_public_clear(paillier_public_key *pub) { mpz_clear(pub->n); }

void paillier_private_clear(paillier_private_key *priv) {
  mpz_clear(priv->lambda);
  mpz_clear(priv->mu);
  mpz_clear(priv->p2);
  mpz_clear(priv->q2);
  mpz_clear(priv->p2invq2);
  mpz_clear(priv->ninv);
  mpz_clear(priv->n);
}

int paillier_public_out_str(FILE *fp, paillier_public_key *pub) {
  int printf_ret, result = 0;

  printf_ret = gmp_fprintf(fp, "%d\n", pub->len);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  DEBUG_MSG("output modulus n\n");
  printf_ret = gmp_fprintf(fp, "%Zx\n", pub->n);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;

  return result;
}

int paillier_private_out_str(FILE *fp, paillier_private_key *priv) {
  int printf_ret, result = 0;

  printf_ret = gmp_fprintf(fp, "%d\n", priv->len);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->lambda);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->mu);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->p2);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->q2);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->p2invq2);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->ninv);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;
  printf_ret = gmp_fprintf(fp, "%Zx\n", priv->n);
  if (printf_ret < 0)
    return printf_ret;
  result += printf_ret;

  return result;
}

int paillier_public_in_str(paillier_public_key *pub, FILE *fp) {
  int scanf_ret, result = 0;

  DEBUG_MSG("importing bit length\n");
  scanf_ret = gmp_fscanf(fp, "%d\n", &(pub->len));
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing modulus\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", pub->n);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;

  return result;
}

int paillier_private_in_str(paillier_private_key *priv, FILE *fp) {
  int scanf_ret, result = 0;

  DEBUG_MSG("importing bit length\n");
  scanf_ret = gmp_fscanf(fp, "%d\n", &(priv->len));
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing lambda\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->lambda);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing mu\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->mu);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing p^2\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->p2);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing q^2\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->q2);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing p^-2 mod q^2\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->p2invq2);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing n^-1 mod 2^len\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->ninv);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;
  DEBUG_MSG("importing n\n");
  scanf_ret = gmp_fscanf(fp, "%Zx\n", priv->n);
  if (scanf_ret < 0)
    return scanf_ret;
  result += scanf_ret;

  return result;
}
