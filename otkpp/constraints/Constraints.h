
#ifndef CONSTRAINTS_H

#include <otkpp/lib/Cloneable.h>

#include <boost/numeric/ublas/vector.hpp>
#include <stdexcept>

using namespace boost::numeric::ublas;

/// A base class for constraints.
class Constraints
{
 public:
  /// Destructor.
  virtual ~Constraints() { }
  
  /// Creates a copy.
  virtual Constraints *clone() const = 0;
  
  virtual bool isFeasible(const vector< double > &x) const = 0;
};

/// Specifies that no constraints are given.
class NoConstraints : public Cloneable< NoConstraints, Constraints >
{
 public:
  bool isFeasible(const vector< double > &x) const { return true; }
};

#define CONSTRAINTS_H

#endif
