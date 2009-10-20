
#ifndef LINMINBFGS_H

#include <otkpp/linalg/InvLBFGSUpdater.h>
#include <otkpp/linalg/QuasiNewtonUpdater.h>
#include <otkpp/localsolvers/native/GradientSolver.h>
#include <otkpp/localsolvers/native/linmin/LineMinimizer.h>

class LineMinimizer;

/// Defines the parameters of a LinminBFGS solver.
struct LinminBFGSSetup : public SolverSetup
{
  const LinminSetup *lmSetup;   ///< line minimizer parameters
  const matrix< double > *H0;   ///< initial Hessian or inverse Hessian approximation
  
  LinminBFGSSetup(const LinminSetup &lmSetup = DefaultLinminSetup(),
                  const matrix< double > &H0 = zero_matrix< double >(0, 0));
  ~LinminBFGSSetup();
  bool isCompatibleWith(const Solver &s) const;
};

/// Implements the BFGS algorithm with line searches.
class LinminBFGS : public GradientSolver
{
 public:
  /// Line search algorithm type.
  enum LinMinType
  {
    FLETCHER,     ///< Fletcher's line search.
    MORE_THUENTE  ///< The More and Thuente line search
  };
  
  /// Constructs a new (L-)BFGS solver.
  /**
   * Constructs a new BFGS solver with the specified line 
   * search algorithm and iteration history length.
   * @param lmType line search algorithm
   * @param iterHistLen the length of iteration history
   *                    (0 means full inverse Hessian approximation)
   */
  LinminBFGS(LinMinType lmType = MORE_THUENTE, int iterHistLen = 0);
  
  ~LinminBFGS();
  
  std::string getName() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  vector< double > d_;
  InvLBFGSUpdater dirUpdater_;
  int iterHistLen_;
  LineMinimizer *lineMinimizer_;
  LinMinType lmType_;
  QuasiNewtonUpdater *matrixUpdater_;
  vector< double > p_;
  vector< double > q_;
  matrix< double > S_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define LINMINBFGS_H

#endif
