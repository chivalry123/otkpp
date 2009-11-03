
#ifndef INVLBFGSUPDATER_H

#include <boost/numeric/ublas/matrix.hpp>
#include <list>

using namespace boost::numeric::ublas;

/// Implements the two-loop recursion inverse L-BFGS formula.
/**
 * This class implements the two-loop recursion inverse L-BFGS 
 * formula. It never explicitly stores the inverse Hessian 
 * approximation. Instead it stores and updates its product 
 * with negative gradient.
 */
class InvLBFGSUpdater
{
 public:
  InvLBFGSUpdater() { }
  
  /// Constructs a new inverse L-BFGS updater.
  /**
   * Constructs a new L-BFGS updater for n-dimensional 
   * inverse Hessian approximations with iteration history 
   * of length m.
   */
  InvLBFGSUpdater(int m, int n);
  
  /// Clears iteration history.
  void clearHistory();
  
  /// Removes the oldest pair from the iteration history.
  void removeOldestPair();
  
  /// Stores a new correction pair.
  void storePair(const vector< double > &p,
                 const vector< double > &q);
  
  /// Takes one update step.
  /**
   * Updates the product of the inverse Hessian approximation 
   * and the negative gradient and returns the updated vector.
   * @param g gradient vector.
   * @param d the vector into which the updated 
   *          product of the inverse Hessian approximation 
   *          and the negative gradient is stored.
   * @return the computed d-vector
   */
  vector< double > &update(const vector< double > &g,
                           vector< double > &d);
 private:
  vector< double > alpha_;
  matrix< double > I_;
  int m_, n_;
  std::list< vector< double > > P_;
  std::list< vector< double > > Q_;
  vector< double > r_;
  vector< double > rho_;
};

#define INVLBFGSUPDATER_H

#endif
