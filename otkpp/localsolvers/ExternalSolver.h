
#ifndef EXTERNALSOLVER_H

#include <otkpp/localsolvers/Solver.h>

/// Defines the interface for external algorithms.
/**
 * This class defines the interface of third-party 
 * solvers for which intermediate results cannot 
 * be obtained. Hence, this class only provides a 
 * method for obtaining final results.
 */
class ExternalSolver : public Solver
{
 public:
  virtual SolverResults solve(const Function &objFunc,
                              const vector< double > &x0,
                              const SolverSetup &solverSetup = DefaultSolverSetup(),
                              const StoppingCriterion *stopCrit = NULL) = 0;
};

#define EXTERNALSOLVER_H

#endif
