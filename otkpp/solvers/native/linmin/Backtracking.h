
#ifndef BACKTRACKING_H

#include <otkpp/localsolvers/native/linmin/LineMinimizer.h>

class Backtracking : public LineMinimizer
{
 public:
  struct Setup : public LineMinimizer::Setup { }; // TODO
   
  Backtracking();
  
  int minimize(const vector< double > &x,
               const vector< double > &d,
               double alpha0,
               double fx,
               const vector< double > &gx,
               double &alpha,
               vector< double > &x_plus,
               double &f_plus,
               vector< double > &g_plus);
 private:
  double alpha0_;
  double mu_;
  double rho_;
  double sigma_;
  
  void doSetup_(const LineMinimizer::Setup &) { }
};

#define BACKTRACKING_H

#endif
