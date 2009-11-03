
#ifndef HOOKEJEEVES_H

#include <otkpp/localsolvers/native/NativeSolver.h>

/// Implements the derivative-free Hooke-Jeeves algorithm.
class HookeJeeves : public NativeSolver
{
 public:
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  double alpha_;
  double delta_;
  double eps_;
  double rho_;
  vector< double > xPlus_;
  vector< double > y_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define HOOKEJEEVES_H

#endif
