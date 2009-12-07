
#ifndef MNEWTON_H

#include <otkpp/localsolvers/native/GradientSolver.h>

class Solver;

/// Implements the Newton method with line searches.
class MNewton : public GradientSolver
{
 public:
  struct State : public Cloneable< State, GradientSolver::State > { };
  
  MNewton();
  ~MNewton();
  
  std::string getName() const;
  const State &getState() const { return state_; }
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  vector< double > d_;
  matrix< double > H_;
  matrix< double > H_chol_;
  LineMinimizer *lineMinimizer_;
  State state_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup,
              const Constraints &C);
};

#define MNEWTON_H

#endif
