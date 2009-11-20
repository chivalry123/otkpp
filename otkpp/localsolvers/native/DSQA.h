
#ifndef DSQA_H

#include "QuadInterp.h"

#include <otkpp/localsolvers/native/NativeSolver.h>

/// An experimental direct search algorithm with quadratic interpolation.
class DSQA : public NativeSolver
{
 public:
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  QuadInterp model_;
  double delta_;
  int m_;
  
  double computeReduction_(const vector< double > &x,
                           const vector< double > &xPlus,
                           const vector< double > &p) const;
  double computeTau_(const vector< double > &d,
                     const vector< double > &p);
  vector< double > &computeTrsRegStep_(const vector< double > &g,
                                       const matrix< double > &H,
                                       double delta,
                                       vector< double > &p);
  vector< double > &computeTrsRegStep2_(const vector< double > &g,
                                        const matrix< double > &H,
                                        double delta,
                                        vector< double > &p);
  NativeSolver::IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define DSQA_H

#endif
