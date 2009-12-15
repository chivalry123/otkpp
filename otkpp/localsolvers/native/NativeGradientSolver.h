
#ifndef NATIVEGRADIENTSOLVER_H

#include <otkpp/localsolvers/GradientSolverBase.h>

class LineMinimizer;

/// Defines a gradient-based solver.
/**
 * This class defines additional methods and attributes 
 * used by all native gradient-based solvers.
 */
class NativeGradientSolver : public GradientSolverBase
{
 public:
  struct State : public NativeSolver::State
  {
    double alpha;
    vector< double > g;
  };
  
  /// Returns the current gradient vector \f$\nabla f(\mathbf{x}_{k})\f$.
  virtual const vector< double > getGradient() const;
  
  virtual const State &getState() const = 0;
  bool hasBuiltInStoppingCriterion() const;
 protected:
  double fPlus_;
  vector< double > gPlus_;
  vector< double > xPlus_;
  
  NativeGradientSolver(bool useFDiffGradient = false);
  
  virtual void doSetup_(const Function &objFunc,
                        const vector< double > &x0,
                        const Solver::Setup &solverSetup,
                        const Constraints &C);
  State &getState_();
};

#define NATIVEGRADIENTSOLVER_H

#endif
