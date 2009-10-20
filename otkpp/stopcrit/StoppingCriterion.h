
#ifndef STOPPINGCRITERION_H

class NativeSolver;

/// Defines a stopping criterion for a minimization algorithm.
class StoppingCriterion
{
 public:
  virtual ~StoppingCriterion() { }
  
  /// Returns the computed value that is tested against the user-given limit.
  virtual double getTestValue(const NativeSolver &s) const = 0;
  
  /// Tests the given solver against this stopping criterion.
  virtual bool test(const NativeSolver &s) const = 0;
};

#define STOPPINGCRITERION_H

#endif
