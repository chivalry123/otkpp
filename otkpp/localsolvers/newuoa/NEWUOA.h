
#ifndef NEWUOA_H

#include <otkpp/localsolvers/Solver.h>

class NEWUOA : public Solver
{
 public:
  NEWUOA() { }
  ~NEWUOA() { }
   
  std::string getName() const;
  
  bool isExternalSolver() const;
  
  boost::shared_ptr< Solver::Results > solve(Function &objFunc,
                                             const vector< double > &x0,
                                             const StoppingCriterion &stopCrit,
                                             const Solver::Setup &solverSetup = Solver::DefaultSetup(),
                                             const Constraints &C = NoConstraints(),
                                             bool timeTest = false);
  
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C) { }
};

#define NEWUOA_H

#endif
