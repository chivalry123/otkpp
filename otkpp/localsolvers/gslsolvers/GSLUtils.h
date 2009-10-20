
#ifndef GSLUTILS_H

#include <boost/numeric/ublas/vector.hpp>
#include <gsl/gsl_vector.h>

using namespace boost::numeric::ublas;

class Function;

/// @file GSLUtils.h

namespace gslutils
{
  /// Converts uBLAS vector to gsl_vector.
  void BoostToGSLVector(const vector < double > &bv, gsl_vector *gv);

  /// Converts gsl_vector to uBLAS vector.
  void GSLToBoostVector(const gsl_vector *gv, vector < double > &bv);
  
  /// a wrapper method for evaluating function values in GSL algorithms
  double f   (const gsl_vector *x, void *params);
  
  /// a wrapper method for evaluating gradients in GSL algorithms
  void   df  (const gsl_vector *x, void *params, gsl_vector *g);

  /// a wrapper method for evaluating both function values and gradients in GSL algorithms
  void   fdf (const gsl_vector *x, void *params, double *f, gsl_vector *g);
  
  /// Sets the function used by f, df and fdf methods.
  void setFunction(const Function &f);
}

#define GSLUTILS_H

#endif
