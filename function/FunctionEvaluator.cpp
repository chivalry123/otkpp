
#include "FunctionEvaluator.h"

double FunctionEvaluator::operator()(const double *x) const
{
  if(evalCounting_)
    evalCounter_++;
  
  return eval_(x);
}

double FunctionEvaluator::operator()(const vector< double > &x) const
{
  if(evalCounting_)
    evalCounter_++;
  
  return eval_(&x[0]);
}

bool FunctionEvaluator::hasSymbolicExpression() const
{
  return false;
}
