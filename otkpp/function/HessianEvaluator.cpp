
#include "HessianEvaluator.h"

matrix< double > &HessianEvaluator::eval(const vector< double > &x,
                                         matrix< double > &H) const
{
  if(evalCounting_ && !usesFiniteDifference())
    evalCounter_++;
  
  return eval_(x, H);
}
