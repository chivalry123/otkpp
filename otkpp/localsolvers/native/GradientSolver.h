
#ifndef GRADIENTSOLVER_H

#include <otkpp/localsolvers/AbstractGradientSolver.h>

class LineMinimizer;

/// Defines a gradient-based solver.
/**
 * This class defines additional methods and attributes 
 * used by all native gradient-based solvers.
 */
class GradientSolver : public AbstractGradientSolver
{
 public:
  virtual const vector< double > getGradient() const;
  bool hasBuiltInStoppingCriterion() const;
 protected:
  vector< double > g_;
  vector< double > xPlus_;
  double fPlus_;
  vector< double > gPlus_;
  
  GradientSolver(bool useFDiffGradient = false);
  
  virtual void setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const SolverSetup &solverSetup,
                      const Constraints &C);
};

#define GRADIENTSOLVER_H

#endif
