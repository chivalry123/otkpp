
#include "GradNormTest.h"
#include "GSLFDFSolver.h"

#ifdef WITH_GSL
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>
#endif

GradNormTest::GradNormTest(double eps) : eps_(eps) { }

double GradNormTest::getTestValue(const NativeSolver &s) const
{
  if(typeid(s) != typeid(GSLFDFSolver &))
    return norm_2(s.getGradient());
#ifdef WITH_GSL
  else
  {
    const gsl_multimin_fdfminimizer *gslSolver = 
      dynamic_cast< const GSLFDFSolver & >(s).getGSLSolver();
    return gsl_blas_dnrm2(gslSolver->gradient);
  }
#endif
}

bool GradNormTest::test(const NativeSolver &s) const
{
  if(!s.usesGradient())
    throw std::runtime_error("no gradient information available");
  
  return getTestValue(s) < eps_;
}
