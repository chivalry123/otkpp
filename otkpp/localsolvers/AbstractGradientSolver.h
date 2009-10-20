
#ifndef ABSTRACTGRADIENTSOLVER_H

#include <otkpp/function/Function.h>
#include <otkpp/localsolvers/native/NativeSolver.h>

/// Defines the interface for gradient-based solvers.
class AbstractGradientSolver : public NativeSolver
{
 public:
  /// Returns the current gradient vector \f$\nabla f(\mathbf{x}_{k})\f$.
  virtual const vector< double > getGradient() const = 0;
 protected:
  Function::DerivEvalType gEvalType_;
  
  AbstractGradientSolver(Function::DerivEvalType gEvalType);
  virtual void setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const SolverSetup &solverSetup,
                      const Constraints &C);
};

#define ABSTRACTGRADIENTSOLVER_H

#endif
