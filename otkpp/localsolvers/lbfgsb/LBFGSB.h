
#ifndef LBFGSB_H

#include <otkpp/localsolvers/AbstractGradientSolver.h>
#include <otkpp/constraints/BoundConstraints.h>

/// Defines parameters for L-BFGS-B.
struct LBFGSBSetup : public SolverSetup
{
  int m;
  
  /// Constructs a new L-BFGS-B setup.
  /**
   * @param m the length of iteration history.
   */
  LBFGSBSetup(int m);
  
  bool isCompatibleWith(const Solver &s) const;
};

/// A wrapper class for Nocedal's L-BFGS-B Fortran implementation.
class LBFGSB : public AbstractGradientSolver
{
 public:
  /// Constructs a new L-BFGS-B solver with the given gradient evaluator type.
  LBFGSB(Function::DerivEvalType gEvalType = Function::DERIV_FDIFF_CENTRAL_2);

  const vector< double > getGradient() const;
  std::string getName() const;
  bool hasBuiltInStoppingCriterion() const;
  bool isGSLSolver() const;
  bool supportsConstraints(const Constraints &C);
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  BoundConstraints constraints_;
  char csave_[60];
  double dsave_[29];
  int isave_[44];
  int lsave_[4];
  double factr_;
  vector< double > g_;
  std::vector< int > iwa_;
  int m_;
  std::vector< int > nbd_;
  double pgtol_;
  char task_[60];
  std::vector< double > wa_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define LBFGSB_H

#endif
