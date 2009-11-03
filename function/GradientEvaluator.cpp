
#include "GradientEvaluator.h"

double *GradientEvaluator::eval(const double *x,
                                double *g) const
{
  if(evalCounting_ && !usesFiniteDifference())
    evalCounter_++;
  
  eval_(x, g);
  return g;
}

vector< double > &GradientEvaluator::eval(const vector< double > &x,
                                          vector< double > &g) const
{
  if(evalCounting_ && !usesFiniteDifference())
    evalCounter_++;
  
  eval_(&x[0], &g[0]);
  return g;
}
