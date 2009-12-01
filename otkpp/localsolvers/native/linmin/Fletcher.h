
#ifndef FLETCHER_H

#include <otkpp/localsolvers/native/linmin/LineMinimizer.h>

/// Implements Fletcher's line search algorithm.
class Fletcher : public LineMinimizer
{
 public:
  struct Setup : public LineMinimizer::Setup
  {
    double eta;
    double mu;
    double chi;
    double tau;
  
    Setup(double eta = 0.1, double mu = 0.01, double chi = 9.0, double tau = 0.05);
    const LineMinimizer::Setup *clone() const;
    bool isCompatibleWith(const LineMinimizer &s) const;
  };

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
  double alphal_, alphat_, alphau_;
  double phil_, phit_;
  double dphil_, dphit_;
  vector< double > xt_;
  vector< double > gt_;
  Setup setup_;
  
  void doSetup_(const LineMinimizer::Setup &s);
  double extrap_safequard_(double alpha);
  double interp_safequard_(double alpha);
};

#define FLETCHER_H

#endif
