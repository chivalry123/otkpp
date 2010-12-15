
#include "Function.h"
#include "GSLUtils.h"

// TODO: declaring this as a global variable prevents 
// accessing it from multiple GSL minimizers
// a pointer-to-member function could be used, 
// but it requires using a gcc-specific extension...
static const Function *f_;

namespace gslutils
{
  void BoostToGSLVector(const vector < double > &bv, gsl_vector *gv)
  {
    for(int i = 0; i < bv.size(); i++)
      gsl_vector_set(gv, i, bv(i));
  }
  
  // TODO: more efficient implementation, avoid copying
  void GSLToBoostVector(const gsl_vector *gv, vector < double > &bv)
  {
    bv = vector< double >(gv->size);
    for(int i = 0; i < gv->size; i++)
      bv(i) = gsl_vector_get(gv, i);
  }
  
  double f(const gsl_vector *x, void *params)
  {
    return (*f_)(x->data);
  }
  
  void df(const gsl_vector *x, void *params, gsl_vector *g)
  {
    f_->g(x->data, g->data);
  }
  
  void fdf(const gsl_vector *x, void *params, double *f, gsl_vector *g)
  {
    *f = (*f_)(x->data);
    f_->g(x->data, g->data);
  }
  
  void setFunction(const Function &f)
  {
    f_ = &f;
  }
}
