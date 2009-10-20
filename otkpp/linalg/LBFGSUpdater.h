
#ifndef LBFGSUPDATER_H

#include <otkpp/linalg/QuasiNewtonUpdater.h>

#include <boost/numeric/ublas/lu.hpp>

// Implements the L-BFGS formula for Hessian approximations.
class LBFGSUpdater
{
 public:
  /// Constructs a new L-BFGS updater.
  /**
   * Constructs a new L-BFGS updater for an n-dimensional 
   * Hessian approximations with iteration history 
   * of length m.
   */
  LBFGSUpdater(int n, int m);
  
  bool computeProduct(const vector< double > &s,
                      const vector< double > &y,
                      const vector< double > &v,
                      vector< double > &Bv);
  
  /// Removes the oldest iterates from iteration history.
  void removeOldest();
  
  /// Processes a new pair of correction vectors s, y.
  /** 
   * @param s represents \f$\mathbf{s}_{k+1}=\mathbf{x}_{k+1}-\mathbf{x}_{k}\f$.
   * @param y represents \f$\mathbf{y}_{k+1}=\mathbf{g}_{k+1}-\mathbf{g}_{k}\f$.
   */
  void updateVectors(const vector< double >&s,
                     const vector< double >&y);
 private:
  int n_, m_;
  const matrix< double > I_;
  matrix< double > invD_;
  matrix< double > L_;
  matrix< double > S_;
  matrix< double > sqrtD_;
  matrix< double > sqrtInvD_;
  matrix< double > SS_;
  matrix< double > Y_;
  
  matrix< double > JJ_;
  matrix< double > JJ_chol_;
  matrix< double > M_;
  permutation_matrix< double > P_;
  vector< double > p_, q_;
  matrix< double > tri_;
  
  void updateInvD_(const vector< double >&s,
                   const vector< double >&y);
  void updateL_(const vector< double > &s,
                const vector< double > &y);
  void updateSS_();
  void updateSY_(const vector< double > &s,
                 const vector< double > &y);
};

#define LBFGSUPDATER_H

#endif
