
#include "FDistToMinTest.h"

FDistToMinTest::FDistToMinTest(double fMin, double eps, bool relative) : 
  fMin_(fMin), eps_(eps), relative_(relative) { }

double FDistToMinTest::getTestValue(const NativeSolver &s) const
{
  if(relative_ == false)
    return s.getFVal() - fMin_;
  else
  {
    double v = (s.getFVal() - fMin_) / fabs(fMin_);
    return v > 0.0 ? v : NAN;
  }
}

bool FDistToMinTest::test(const NativeSolver &s) const
{
  return (getTestValue(s) < eps_);
}
