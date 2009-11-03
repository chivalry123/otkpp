
#ifndef MNEWTON_H

#include <otkpp/localsolvers/native/GradientSolver.h>

class Solver;

/// Implements the Newton method with line searches.
class MNewton : public GradientSolver
{
 public:
  MNewton();
  ~MNewton();
  
  std::string getName() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  vector< double > d_;
  matrix< double > H_;
  matrix< double > H_chol_;
  LineMinimizer *lineMinimizer_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define MNEWTON_H

#endif
