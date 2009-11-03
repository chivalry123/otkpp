
#ifndef STEIHAUGSOLVER_H

#include <otkpp/function/Function.h>
#include <otkpp/localsolvers/native/trsreg/TrustRegionSolver.h>

/// Implements Steihaug's trust region algorithm.
class SteihaugSolver : public TrustRegionSolver
{
 public:
  SteihaugSolver();
  
  vector< double > &computeStep(const vector< double > &x,
                                double fx,
                                const vector< double > &g,
                                const matrix< double > &H,
                                vector< double > &p,
                                bool &nonzeroStep,
                                vector< double > &xPlus,
                                double &fxPlus,
                                const vector< double > *Hg = NULL);
 private:
  double computeTau_(const vector< double > &d,
                     const vector< double > &p);
  
  void doSetup_();
};

#define STEIHAUGSOLVER_H

#endif

