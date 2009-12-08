
#ifndef PARTAN_H

#include <otkpp/localsolvers/native/GradientSolver.h>

class LineMinimizer;

/// Implements the PARTAN (parallel tangents) algorithm.
class PARTAN : public GradientSolver
{
 public:
  struct State : public Cloneable< State, GradientSolver::State > { };
  
  PARTAN();
  ~PARTAN();
  
  std::string getName() const;
  const State &getState() const { return state_; }
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  vector< double > d_;
  LineMinimizer *lineMinimizer_;
  int j_;
  int iterState_;
  vector< double > xMinus_;
  double fMinus_;
  vector< double > gMinus_;
  State state_;
  vector< double > y_;
  
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C);
  IterationStatus iterate_();
};

#define PARTAN_H

#endif
