
// TODO: work in progress...

#ifndef NATIVENEWUOA_H

/*#include "NativeSolver.h"

#include <vector>

struct NativeNEWUOASetup : public SolverSetup
{
  double m;
  double rho0;
  double rhoFinal;
  
  bool isCompatibleWith(const Solver &s) const;
};

class NativeNEWUOA : public NativeSolver
{
 public:
  
 private:
  matrix< double > A_;
  vector< double > b_;
  double c_;
  double crvmin_;
  double delta_;
  double rho_;
  NativeNEWUOASetup solverSetup_;
  std::vector< vector< double > > X_;
  
  void computeDirection_(int n, vector< double > &d);
  void computeInitialModel_(const Function &f, matrix< double > &A, vector< double > &b, double &c);
  double Q_(const vector< double > &x);
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup = DefaultSolverSetup());
  void TRSolve_();
};*/

#define NATIVENEWUOA_H

#endif
