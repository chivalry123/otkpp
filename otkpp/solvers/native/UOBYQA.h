
#ifndef UOBYQA_H

//#include "SteihaugSolver.h"
#include "QuadInterp.h"

#include <otkpp/localsolvers/native/NativeSolver.h>

/// A direct search algorithm based on Powell's UOBYQA.
class UOBYQA : public NativeSolver
{
 public:
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  QuadInterp model_;
  double delta_;
  double eps_;
  int m_;
  vector< double > d_;
  double dNorm_;
  double lambda_;
  double rho_;
  double rhoFinal_;
  int moveIndex_;
  int numMUpdates_;
  double M_;
  std::vector< std::pair< int, double > > J_;
  vector< double > xPlus_;
  double fXPlus_;
  
  //void computeCenterPoint_();
  /*void computeInitModelParams_(const vector< double > &x0);
  void computeLagrangeCoeff_first_(int i, const vector< double > &x0);
  void computeLagrangeCoeff_last_(const vector< double > &x0, double denom, int i, int j, int k);
  void computeLagrangeCoeff_hat_(const vector< double > &x0, double denom1, double denom2, int j);*/
  double computeMinEigValEstim_();
  void computeModelStep_(int j, vector< double > &d, double &lj);
  double computeReduction_(const vector< double > &x,
                           const vector< double > &xPlus,
                           const vector< double > &p) const;
  //void generateInitialPoints_(const vector< double > &x0);
  double computeTau_(const vector< double > &d,
                     const vector< double > &p);
  vector< double > &computeTrsRegStep_(const vector< double > &g,
                                       const matrix< double > &H,
                                       double delta,
                                       vector< double > &p);
  NativeSolver::IterationStatus iterate_();
  NativeSolver::IterationStatus iterate2_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
  void updateM_();
};

#define UOBYQA_H

#endif
