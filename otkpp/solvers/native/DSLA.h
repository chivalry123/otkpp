
#ifndef DSLA_H

#include <otkpp/interpolation/LinInterp.h>
/*#include <otkpp/linalg/BFGSUpdater.h>
#include <otkpp/linalg/SR1Updater.h>*/

#include <otkpp/solvers/native/NativeSolver.h>

class DSLA : public NativeSolver
{
 public:
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  LinInterp model_;
  double delta_;
  matrix< double > H_;
  int m_;
  vector< double > p_;
  //SR1Updater HUpdater_;
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
  void updateHessianApprox_(const vector< double > &p,
                            double fxPlus,
                            double fxMinus,
                            double fx);
};

#define DSLA_H

#endif
