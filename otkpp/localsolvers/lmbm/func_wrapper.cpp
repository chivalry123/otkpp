
#include "func_wrapper.h"
#include "Function.h"

static const Function *objfunc;

void func_wrapper_(int *n, double *x, double *f, double *g)
{
  vector< double > x_(*n);
  for(int i = 0; i < *n; i++)
    x_[i] = x[i];
  *f = (*objfunc)(x_);
  vector< double > g_(*n);
  objfunc->g(x_, g_);
  for(int i = 0; i < *n; i++)
    g[i] = g_[i];
}

void set_obj_func(const Function *f)
{
  objfunc = f;
}
