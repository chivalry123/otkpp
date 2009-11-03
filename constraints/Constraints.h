
#ifndef CONSTRAINTS_H

#include <otkpp/lib/Cloneable.h>

#include <stdexcept>

/// A base class for constraints.
class Constraints
{
 public:
  /// Destructor.
  virtual ~Constraints() { }
  
  /// Creates a copy.
  virtual Constraints *clone() const = 0;
};

/// Specifies that no constraints are given.
class NoConstraints : public Cloneable< NoConstraints, Constraints > { };

#define CONSTRAINTS_H

#endif
