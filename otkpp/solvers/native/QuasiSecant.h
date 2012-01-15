
#ifndef QUASISECANT_H

#include <otkpp/localsolvers/native/NativeSolver.h>

class QuasiSecant : public NativeSolver
{
 public:
 private:
  void addMatrixCol_(const vector< double > &c, matrix< double > &M);
  void computeDescentDirection_();
  IterationStatus iterate_();
  void polyMin_(const matrix< double > &P, vector< double > &pMin);
  void computeQuasiSecant(const vector< double > &x,
                          const vector< double > &g,
                          double h);
  void removeMatrixCol_(int j, matrix< double > &M);
};

#define QUASISECANT_H

#endif
