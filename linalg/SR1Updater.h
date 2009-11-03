
#ifndef SR1UPDATER_H

#include <otkpp/linalg/QuasiNewtonUpdater.h>

#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

/// Implements the SR1 formula for updating Hessian approximations.
class SR1Updater : public QuasiNewtonUpdater
{
 public:
  matrix< double > &update(const vector< double > &p,
                           const vector< double > &q,
                           matrix< double > &H);
};

#define SR1UPDATER_H

#endif
