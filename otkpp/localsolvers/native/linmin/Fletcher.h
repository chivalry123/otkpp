
#ifndef FLETCHER_H

#include <otkpp/localsolvers/native/linmin/LineMinimizer.h>

struct FletcherSetup : public LinminSetup
{
  double eta;
  double mu;
  double chi;
  double tau;
  
  FletcherSetup(double eta = 0.1, double mu = 0.01, double chi = 9.0, double tau = 0.05);
  const LinminSetup *clone() const;
  bool isCompatibleWith(const LineMinimizer &s) const;
};

/// Implements Fletcher's line search algorithm.
class Fletcher : public LineMinimizer
{
 public:
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
  FletcherSetup setup_;
  
  void doSetup_(const LinminSetup &s);
  double extrap_safequard_(double alpha);
  double interp_safequard_(double alpha);
};

#define FLETCHER_H

#endif
