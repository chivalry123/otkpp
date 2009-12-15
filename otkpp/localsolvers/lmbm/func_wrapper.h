
#ifndef FUNC_WRAPPER_H

#include "lmbm_utils.h"

#ifdef __cplusplus
extern "C" {
#endif
void func_wrapper_(int *n, double *x, double *f, double *g);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class Function;
void set_obj_func(const Function *f);
#endif

#define FUNC_WRAPPER_H

#endif
