#ifndef SVD_POWER_METHOD_H_
#define SVD_POWER_METHOD_H_

#include <gmp.h>
#include <stdbool.h> 
#include <stdint.h>

void svd(mpf_t *eigenspace, bool isReachEp, bool isInit, mpf_t *eigenspace_prev, mpf_t *vector, uint32_t dim, int times_p, uint32_t k);

#endif /* SVD_POWER_METHOD_H_ */
