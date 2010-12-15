
#ifndef DOGLEGSOLVER_H

#include <otkpp/solvers/native/trsreg/TrustRegionSolver.h>

/// Implements the dogleg trust region algorithm.
class DoglegSolver : public TrustRegionSolver
{
 public:
   DoglegSolver();
   
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
  double computeTau_(const vector< double > &pB,
                     const vector< double > &pU);
  
  void doSetup_();
};

#define DOGLEGSOLVER_H

#endif
