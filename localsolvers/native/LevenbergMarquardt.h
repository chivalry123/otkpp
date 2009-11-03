
#ifndef LEVENBERGMARQUARDT_H

/*#include "GradientSolver.h"

class Solver;

// TODO

class LevenbergMarquardtSetup : public SolverSetup
{
 public:
  bool isCompatibleWith(const Solver &s) const;
};

class LevenbergMarquardt : public GradientSolver
{
 public:
  LevenbergMarquardt();
  ~LevenbergMarquardt();
  
  std::string getName() const;
  
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
 private:
  vector< double > d_;
  double eps_;
  matrix< double > H_;
  matrix< double > H_mod_;
  matrix< double > I_;
};*/

#define LEVENBERGMARQUARDT_H

#endif
