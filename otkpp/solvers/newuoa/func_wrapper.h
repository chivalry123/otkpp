
#ifndef FUNC_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif
  void calfun_(int *n, double *x, double *f);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class Function;
void set_newuoa_obj_func(const Function *f);
#endif

#define FUNC_WRAPPER_H

#endif
