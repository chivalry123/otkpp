
#ifndef STEIHAUGSR1_H

#include <otkpp/linalg/SR1Updater.h>
#include <otkpp/solvers/native/NativeGradientSolver.h>
#include <otkpp/solvers/native/trsreg/SteihaugSolver.h>

/// Implements the BFGS algorithm with Steihaug's trust region method.
class SteihaugSR1 : public NativeGradientSolver
{
 public:
  struct State : public Cloneable< State, NativeGradientSolver::State > { };
  
  SteihaugSR1();
  ~SteihaugSR1();

  std::string getName() const;
  const State &getState() const { return state_; }
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  matrix< double > H_;
  SR1Updater matrixUpdater_;
  vector< double > p_;
  vector< double > q_;
  State state_;
  SteihaugSolver trSolver_;
  
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C);
  IterationStatus iterate_();
};

#define STEIHAUGSR1_H

#endif

