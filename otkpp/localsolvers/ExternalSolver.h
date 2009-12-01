
#ifndef EXTERNALSOLVER_H

#include <otkpp/localsolvers/Solver.h>

/// Defines the interface for external algorithms.
/**
 * This class defines the interface of external 
 * solvers for which intermediate results cannot 
 * be obtained. Hence, this class only provides a 
 * method for obtaining final results.
 */
class ExternalSolver : public Solver
{
 public:
  virtual SolverResults solve(const Function &objFunc,
                              const vector< double > &x0,
                              const Solver::Setup &solverSetup = Solver::DefaultSetup(),
                              const Constraints &C = NoConstraints(),
                              const StoppingCriterion *stopCrit = NULL,
                              bool timeTest = false) = 0;
};

#define EXTERNALSOLVER_H

#endif
