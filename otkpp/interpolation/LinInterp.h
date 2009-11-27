
#ifndef LININTERP_H

#include <otkpp/function/Function.h>

#include <boost/numeric/ublas/vector.hpp>
#include <vector>

class LinInterp
{
 public:
  LinInterp();
  LinInterp(const Function &f, const vector< double > &xb, double delta);
  
  double eval(const vector< double > &d) const;
  double evalLagrangian(int j, const vector< double > &d) const;
  const vector< double > &getG() const;
  double getLowestF() const;
  int getLowestIndex() const;
  const vector< double > &getLowestX() const;
  const vector< double > &getOrigin() const;
  const std::vector< vector< double > > &getX() const;
  void setOrigin(int xbi);
  void testInvariants();
  bool updatePoint(const vector< double > &x, int j);
  bool updatePoint(const vector< double > &x, double fx, int j);
 private:
  Function f_;
  int n_, m_;
  
  std::vector< vector< double > > X_;
  std::vector< double > F_;
  
  vector< double > xb_;
  
  double c_;
  vector< double > g_;
  
  std::vector< double > cl_;
  std::vector< vector< double > > gl_;
  
  int xiLowest_;
  
  void initialize_(const vector< double > &xb, double delta);
};

#define LININTERP_H

#endif
