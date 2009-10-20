
#ifndef GSLFDFSOLVER_H

#include <otkpp/localsolvers/AbstractGradientSolver.h>
#include <otkpp/localsolvers/gslsolvers/GSLUtils.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <gsl/gsl_multimin.h>
#include <typeinfo>

using namespace boost::numeric::ublas;

/// Defines the parameters of a GSLDFSolver.
struct GSLFDFSolver_setup : public SolverSetup
{
  double stepSize;  ///< initial step size for line searches
  double tol;       ///< tolerance for line search stopping criterion
  
  bool isCompatibleWith(const Solver &s) const;
  
  GSLFDFSolver_setup(double stepSize, double tol) : stepSize(stepSize), tol(tol) { }
};

/// A wrapper class for gsl_multimin_fdfminimizer.
/**
 * This class is a wrapper class for gsl_multimin_fdfminimizer. 
 * It conforms to GSL version 1.11.
 */
class GSLFDFSolver : public AbstractGradientSolver
{
 public:
  /// Constructs a new GSL solver with the given name.
  /**
   * Constructs a new GSL solver with the given name.
   * @param gslName One of the following:
   *  - conjugate_fr
   *  - conjugate_pr
   *  - vector_bfgs
   *  - vector_bfgs2
   *  - steepest_descent
   * @param gEvalType gradient evaluator type to be used with this solver.
   */
  GSLFDFSolver(const std::string &gslName,
               Function::DerivEvalType gEvalType = Function::DERIV_SYMBOLIC);
  
  /// Constructs a new GSL solver with the given type.
  /**
   * Constructs a new GSL solver with the given type.
   * @param type see the GSL documentation
   * @param gEvalType gradient evaluator type to be used with this solver.
   */
  GSLFDFSolver(const gsl_multimin_fdfminimizer_type *type,
               Function::DerivEvalType gEvalType = Function::DERIV_SYMBOLIC);
  
  ~GSLFDFSolver();
  
  double getFVal() const;
  const vector< double > getGradient() const;
  
  /// Returns the wrapper gsl_multimin_fdfminimizer object.
  const gsl_multimin_fdfminimizer *getGSLSolver() const;
  
  std::string getName() const;
  const vector< double > getX() const;
  bool hasBuiltInStoppingCriterion() const { return false; }
  bool isGSLSolver() const { return true; }
  void restart();
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  gsl_multimin_function_fdf gslFunction_;
  gsl_multimin_fdfminimizer *gslSolver_;
  const gsl_multimin_fdfminimizer_type *type_;
  
  NativeSolver::IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup = DefaultSolverSetup(),
              const Constraints &C = NoConstraints());
};

#define GSLFDFSOLVER_H

#endif
