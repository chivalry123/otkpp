
#ifndef SYMBOLICGRADIENTEVALUATOR_H

#include <otkpp/lib/Cloneable.h>
#include <otkpp/function/GradientEvaluator.h>
#include <otkpp/function/SymbolicFunctionEvaluator.h>

/// Implements a symbolic gradient evaluator.
/**
 * This class implements a symbolic gradient evaluator using 
 * GNU libmatheval. Allocated evaluators are reference-counted.
 */
class SymbolicGradientEvaluator : public Cloneable< SymbolicGradientEvaluator, GradientEvaluator >
{
 public:
  /// Constructs a symbolic gradient evaluator by using an existing symbolic function evaluator.
  /**
   * This constructor takes an existing symbolic function evaluator 
   * and creates n partial derivative evaluators, where n is the number 
   * of variables in the function evaluator. The reference count 
   * of the function evaluator is incremented.
   */
  SymbolicGradientEvaluator(SymbolicFunctionEvaluator *fe);
  
  /// Constructs a symbolic gradient evaluator from an existing symbolic gradient evaluator.
  /**
   * This constructor generates a copy of en existing symbolic 
   * gradient evaluator. The reference count of the souce 
   * gradient evaluator is incremented.
   */
  SymbolicGradientEvaluator(const SymbolicGradientEvaluator &eval);
  
  /// Destroys this evaluator and deallocates memory associated with it.
  ~SymbolicGradientEvaluator();
  
  /// Assigns an existing evaluator to this evaluator.
  SymbolicGradientEvaluator &operator=(const SymbolicGradientEvaluator &eval);
  
  /// Returns the ith libmatheval partial derivative evaluator.
  const void *getEvalPtr(int i);
  
  /// Returns the number of variables.
  /**
   * Returns the number of variables in the function evaluator 
   * this gradient evaluator is associated with.
   */
  int getN() const { return n_; }
  
  bool usesFiniteDifference() const { return false; }
 private:
  SymbolicFunctionEvaluator *fEval_;
  void **gEval_;
  int n_;
  int *refCount_;
  
  double *eval_(const double *x,
                double *g) const;
};

#define SYMBOLICGRADIENTEVALUATOR_H

#endif
