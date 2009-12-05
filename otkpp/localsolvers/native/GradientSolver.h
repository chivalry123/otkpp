
#ifndef GRADIENTSOLVER_H

#include <otkpp/localsolvers/GradientSolverBase.h>

class LineMinimizer;

/// Defines a gradient-based solver.
/**
 * This class defines additional methods and attributes 
 * used by all native gradient-based solvers.
 */
class GradientSolver : public GradientSolverBase
{
 public:
  struct State : public NativeSolver::State
  {
    vector< double > g;
  };
  
  virtual const vector< double > getGradient() const;
  virtual const State &getState() const = 0;
  bool hasBuiltInStoppingCriterion() const;
 protected:
  double fPlus_;
  vector< double > gPlus_;
  vector< double > xPlus_;
   
  GradientSolver(bool useFDiffGradient = false);
  
  State &getState_();
  virtual void setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const Solver::Setup &solverSetup,
                      const Constraints &C);
};

#define GRADIENTSOLVER_H

#endif
