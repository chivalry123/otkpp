
#include "AbstractGradientSolver.h"

AbstractGradientSolver::AbstractGradientSolver(Function::DerivEvalType gEvalType) : 
  gEvalType_(gEvalType) { }

void AbstractGradientSolver::setup_(const Function &objFunc,
                                    const vector< double > &x0,
                                    const SolverSetup &solverSetup,
                                    const Constraints &C)
{
#ifdef WITH_LIBMATHEVAL
  if(gEvalType_ != Function::DERIV_SYMBOLIC)
#endif
    objFunc_ = objFunc.createCopy(gEvalType_);
  objFunc_.resetEvalCounters();
  objFunc_.enableEvalCounting();
  NativeSolver::setup_(objFunc_, x0, solverSetup);
}
