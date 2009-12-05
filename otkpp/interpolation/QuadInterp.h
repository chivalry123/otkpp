
#ifndef QUADINTERP_H

#include <otkpp/function/Function.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <vector>

using namespace boost::numeric::ublas;

/// Represents a quadratic model interpolating the objective function.
class QuadInterp
{
 public:
  QuadInterp();
  QuadInterp(const Function &f, const vector< double > &xb, double delta);
  
  double eval(const vector< double > &d);
  double evalLagrangian(int j, const vector< double > &d);
  double getC() const;
  double getCl(int i) const;
  const std::vector< double > &getF() const;
  const vector< double > &getG() const;
  const vector< double > &getGl(int i) const;
  const matrix< double > &getH() const;
  const matrix< double > &getHl(int i) const;
  double getLowestF() const;
  int getLowestIndex() const;
  const matrix_column< const matrix< double > > getLowestX() const;
  const vector< double > &getOrigin() const;
  const matrix< double > &getX() const;
  void setOrigin(int xbi);
  void test();
  void testInvariants();
  bool updatePoint(const vector< double > &x, int j);
  bool updatePoint(const vector< double > &x, double fx, int j);
 private:
  const Function *f_;
  int n_, m_;
  
  //std::vector< vector< double > > X_;
  matrix< double > X_;
  std::vector< double > F_;
  
  vector< double > xb_;
  
  double c_;
  vector< double > g_;
  matrix< double > H_;
  
  std::vector< double > cl_;
  std::vector< vector< double > > gl_;
  std::vector< matrix< double > > Hl_;

  std::vector< double > cl_hat_;
  std::vector< vector< double > > gl_hat_;
  std::vector< matrix< double > > Hl_hat_;
  
  int xiLowest_;
  
  void initialize_(const vector< double > &xb, double delta);
  void computeLagrangeCoeff_first_(int i, const vector< double > &x0);
  void computeLagrangeCoeff_last_(const vector< double > &x0, double denom, int i, int j, int k);
  void computeLagrangeCoeff_hat_(const vector< double > &x0, double denom1, double denom2, int j);
  void printInfo_();
};

#define QUADINTERP_H

#endif
