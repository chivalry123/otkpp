
#include "GradientSolver.h"
#include "LevenbergMarquardt.h"

#include <typeinfo>

/*LevenbergMarquardtSetup::LevenbergMarquardtSetup()
{
}

LevenbergMarquardt::~LevenbergMarquardt()
{
}

bool LevenbergMarquardtSetup::isCompatibleWith(const Solver &s) const
{
  return (typeid(s) == typeid(const LevenbergMarquardt &));
}

LevenbergMarquardt::LevenbergMarquardt()
{
}

LevenbergMarquardt::~LevenbergMarquardt()
{
}

std::string LevenbergMarquardt::getName() const
{
  return "LMS";
}

bool LevenbergMarquardt::isGSLSolver() const
{
  return false;
}

bool LevenbergMarquardt::usesGradient() const
{
  return true;
}

bool LevenbergMarquardt::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus LevenbergMarquardt::iterate_()
{
}

void LevenbergMarquardt::setup_(const Function &objFunc,
                                const vector< double > &x0,
                                const SolverSetup &solverSetup,
                                const Constraints &C)
{
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
}*/
