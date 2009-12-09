
#ifndef LINMINBFGS_H

#include <otkpp/linalg/InvLBFGSUpdater.h>
#include <otkpp/linalg/QuasiNewtonUpdater.h>
#include <otkpp/localsolvers/native/GradientSolver.h>
#include <otkpp/localsolvers/native/linmin/LineMinimizer.h>

class LineMinimizer;

/// Implements the BFGS algorithm with line searches.
class LinminBFGS : public GradientSolver
{
 public:
  /// Defines the parameters of a LinminBFGS solver.
  struct Setup : public Cloneable< Setup, Solver::Setup >
  {
    boost::shared_ptr< LineMinimizer::Setup > lmSetup; ///< line minimizer parameters
    matrix< double > H0;                               ///< initial Hessian or inverse Hessian approximation
  
    Setup(const LineMinimizer::Setup &lmSetup = LineMinimizer::DefaultSetup(),
          const matrix< double > &H0 = zero_matrix< double >(0, 0));
    
    bool isCompatibleWith(const Solver &s) const;
  };

  struct State : public Cloneable< State, GradientSolver::State >
  {
    matrix< double > H;
  };
  
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
  const State &getState() const { return state_; }
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
  State state_;
  
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C);
  IterationStatus iterate_();
};

#define LINMINBFGS_H

#endif
