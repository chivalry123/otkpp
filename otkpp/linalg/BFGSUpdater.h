
#ifndef BFGSUPDATER_H

#include <otkpp/linalg/QuasiNewtonUpdater.h>

#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

/// Implements the standard BFGS update formula.
class BFGSUpdater : public QuasiNewtonUpdater
{
 public:
  /// BFGS update type.
  enum Type
  {
    DIRECT,  /// update Hessian approximation
    INVERSE  /// update Inverse Hessian approximation
  };
  
  /// Constructs a new BFGS updater of the given type.
  BFGSUpdater(Type type = INVERSE);
  
  matrix< double > &update(const vector< double > &p,
                           const vector< double > &q,
                           matrix< double > &H);
 private:
  vector< double > Hq_;
  Type type_;
};

#define BFGSUPDATER_H

#endif
