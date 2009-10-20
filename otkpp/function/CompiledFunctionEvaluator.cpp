
#include "CompiledFunctionEvaluator.h"

CompiledFunctionEvaluator::CompiledFunctionEvaluator(
  double (*f)(const vector< double > &x)) : f_(f) { }

double CompiledFunctionEvaluator::f(const vector< double > &x) const
{
  return (*f_)(x);
}
