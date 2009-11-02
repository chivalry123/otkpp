
#ifndef LMBMSOLVER_H

#include "ExternalSolver.h"

/// Defines parameters for the LMBM algorithm.
struct LMBM_setup : public SolverSetup
{
  int mc;          ///< maximum number of stored corrections, mc >= 3
  double tolf;     ///< tolerance for change of function values
  double tolf2;    ///< second Tolerance for change of function values
  double tolb;     ///< tolerance for the function value
  double tolg;     ///< tolerance for the first termination criterion
  double tolg2;    ///< tolerance for the second termination criterion
  double eta;      ///< distance measure parameter, eta >= 0
  double epsl;     ///< line search parameter, 0 < epsl < 0.25
  int mit;         ///< maximum number of iterations
  int mfe;         ///< maximum number of function evaluations
  int mos;         ///< exponent for distance measure
  int mtesf;       ///< maximum number of iterations with changes of function values smaller than tolf
  int iprint;      ///< printout specification
  double xmax;     ///< maximum stepsize, xmax > 1
  
  /// selection of the method
  /**
   * - 0  - Limited memory bundle method.
   * - 1  - L-BFGS bundle method.
   */
  int method;
  
  /// selection of the scaling
  /**
   * - 0  - Scaling at every iteration with STU/UTU.
   * - 1  - Scaling at every iteration with STS/STU.
   * - 2  - Interval scaling with STU/UTU.
   * - 3  - Interval scaling with STS/STU.
   * - 4  - Preliminary scaling with STU/UTU.
   * - 5  - Preliminary scaling with STS/STU.
   * - 6  - No scaling.
   */
  int iscale;
  
  /// Default constructor.
  LMBM_setup();
  
  bool isCompatibleWith(const Solver &s) const { return true; }
};

/// A wrapper class for the LMBM Fortran implementation by N. Karmitsa.
class LMBM : public ExternalSolver
{
 public:
  LMBM();
  ~LMBM();
  
  std::string getName() const { return "LMBM"; }
  bool hasBuiltInStoppingCriterion() const { return true; }
  bool isGSLSolver() const { return false; }
  bool isExternalSolver() const { return true; }
  /*void setup(const Function &objFunc,
             const vector< double > &x0,
             const SolverSetup &solverSetup = DefaultSolverSetup()) { }*/
  SolverResults solve(const Function &objFunc,
                      const vector< double > &x0,
                      const SolverSetup &solverSetup = DefaultSolverSetup(),
                      const Constraints &C = NoConstraints(),
                      const StoppingCriterion *stopCrit = NULL,
                      bool timeTest = false);
  bool usesGradient() const { return true; }
  bool usesHessian() const { return true; }
 private:
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C) { }
};

#define LMBMSOLVER_H

#endif
