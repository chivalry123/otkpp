
#ifndef DSQA_H

#include <otkpp/interpolation/QuadInterp.h>

#include <otkpp/localsolvers/native/NativeSolver.h>

/// An experimental direct search algorithm with quadratic interpolation (based on Powell's UOBYQA).
class DSQA : public NativeSolver
{
 public:
  struct State : public NativeSolver::State
  {
    QuadInterp model_;
    double delta_;
    int m_;
    vector< double > p_;
    vector< double > xPlus_;
  };
  
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  // TODO: move these to State
  QuadInterp model_;
  double delta_;
  int m_;
  vector< double > p_;
  vector< double > xPlus_;
  
  double computeReduction_(const vector< double > &x,
                           const vector< double > &xPlus,
                           double f,
                           double fPlus,
                           const vector< double > &p) const;
  double computeTau_(const vector< double > &d,
                     const vector< double > &p);
  vector< double > &computeTrsRegStep_(const vector< double > &g,
                                       const matrix< double > &H,
                                       double delta,
                                       vector< double > &p);
  NativeSolver::IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup,
              const Constraints &C);
};

#define DSQA_H

#endif
