
#ifndef FDIFFGRADIENTEVALUATOR_H

#include <otkpp/lib/Cloneable.h>
#include <otkpp/function/FunctionEvaluator.h>
#include <otkpp/function/GradientEvaluator.h>

/// Implements a finite-difference gradient evaluator.
class FDiffGradientEvaluator : public Cloneable< FDiffGradientEvaluator, GradientEvaluator >
{
 public:
  /// Finite-difference evaluator type.
  enum Type
  {
    CENTRAL_2,  ///< two-point central difference
    CENTRAL_4   ///< four-point central difference
  };
  
  /// Constructs a new finite-difference gradient evaluator by using an existing function evaluator.
  FDiffGradientEvaluator(Type type, FunctionEvaluator *fe);
  
  /// Constructs a new finite-difference gradient evaluator from an existing finite-difference gradient evaluator.
  FDiffGradientEvaluator(const FDiffGradientEvaluator &e);
  
  /// Destructor.
  ~FDiffGradientEvaluator();
  
  /// Returns the number of variables.
  int getN() const;
  
  bool usesFiniteDifference() const;
 private:
  FunctionEvaluator *fEval_;
  Type type_;
  double *x_hi_;
  
  double *eval_(const double *x,
                double *g) const;
  double *eval_central_2(const double *x,
                         double *g) const;
  double *eval_central_4(const double *x,
                         double *g) const;
};

#define FDIFFGRADIENTEVALUATOR_H

#endif
