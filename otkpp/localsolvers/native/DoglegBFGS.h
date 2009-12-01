
#ifndef DOGLEGBFGS_H

#include <otkpp/linalg/BFGSUpdater.h>
#include <otkpp/linalg/LBFGSUpdater.h>
#include <otkpp/localsolvers/native/trsreg/DoglegSolver.h>
#include <otkpp/localsolvers/native/GradientSolver.h>

/// Implements the BFGS algorithm with the "dogleg" strategy.
class DoglegBFGS : public GradientSolver
{
 public:
  DoglegBFGS();
  ~DoglegBFGS();

  std::string getName() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  DoglegSolver trSolver_;
  BFGSUpdater matrixUpdater_;
  //LBFGSUpdater *lmatrixUpdater_;
  vector< double > p_;
  vector< double > q_;
  matrix< double > H_;
  
  vector< double > Hg_;

  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup,
              const Constraints &C);
};

#define DOGLEGBFGS_H

#endif
