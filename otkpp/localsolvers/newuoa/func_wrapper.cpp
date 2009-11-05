
#include "func_wrapper.h"
#include "Function.h"

static const Function *objfunc;

void calfun_(int *n, double *x, double *f)
{
  vector< double > x_(*n);
  for(int i = 0; i < *n; i++)
    x_[i] = x[i];
  *f = (*objfunc)(x_);
}

void set_newuoa_obj_func(const Function *f)
{
  objfunc = f;
}
