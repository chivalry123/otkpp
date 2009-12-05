
#include "GradientSolver.h"

#ifdef WITH_LIBMATHEVAL
GradientSolver::GradientSolver(bool useFDiffGradient) : 
  GradientSolverBase(useFDiffGradient == false ? Function::DERIV_SYMBOLIC : Function::DERIV_FDIFF_CENTRAL_2) { }
#else
GradientSolver::GradientSolver(bool useFDiffGradient) : 
  GradientSolverBase(Function::DERIV_FDIFF_CENTRAL_2) { }
#endif

const vector< double > GradientSolver::getGradient() const
{
  return getState().g;
}

GradientSolver::State &GradientSolver::getState_()
{
  return const_cast< GradientSolver::State & >(getState());
}

bool GradientSolver::hasBuiltInStoppingCriterion() const
{
  return false;
}

void GradientSolver::setup_(const Function &objFunc,
                            const vector< double > &x0,
                            const Solver::Setup &solverSetup,
                            const Constraints &C)
{
  GradientSolverBase::setup_(objFunc, x0, solverSetup, C);
  getState_().g.resize(objFunc.getN());
  objFunc.g(x0, getState_().g);
}
