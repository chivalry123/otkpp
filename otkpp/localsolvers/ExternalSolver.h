
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
  virtual boost::shared_ptr< Solver::Results > solve(Function &objFunc,
                                                     const vector< double > &x0,
                                                     const StoppingCriterion &stopCrit,
                                                     const Solver::Setup &solverSetup = Solver::DefaultSetup(),
                                                     const Constraints &C = NoConstraints(),
                                                     bool timeTest = false) = 0;
};

#define EXTERNALSOLVER_H

#endif
