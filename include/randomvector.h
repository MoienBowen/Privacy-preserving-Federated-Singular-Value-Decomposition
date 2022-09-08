#ifndef RANDOMVECTOR_H_
#define RANDOMVECTOR_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    FILE     *file;
    uint64_t *array;
    int       offset;
}
ruv_buffer_t;

void  ruv_init(ruv_buffer_t *buffer);
void  ruv_uniform(ruv_buffer_t *buffer, double *x);
void  ruv_generate(ruv_buffer_t *buffer, int ndimensions, double vector[]);
void ruv_free(ruv_buffer_t *buffer);

#ifdef __cplusplus
}
#endif
#endif /* RANDOMVECTOR_H_ */
