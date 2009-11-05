
#ifndef NEWUOA_H

#include <otkpp/localsolvers/ExternalSolver.h>

class NEWUOA : public ExternalSolver
{
 public:
  NEWUOA() { }
  ~NEWUOA() { }
   
  std::string getName() const;
  
  bool isExternalSolver() const;
  
  SolverResults solve(const Function &objFunc,
                      const vector< double > &x0,
                      const SolverSetup &solverSetup = DefaultSolverSetup(),
                      const Constraints &C = NoConstraints(),
                      const StoppingCriterion *stopCrit = NULL,
                      bool timeTest = false);
  
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C) { }
};

#define NEWUOA_H

#endif
