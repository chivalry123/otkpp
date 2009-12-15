
#ifndef GSLFDFSOLVER_H

#include <otkpp/localsolvers/native/NativeGradientSolver.h>
#include <otkpp/localsolvers/gslsolvers/GSLUtils.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <gsl/gsl_multimin.h>
#include <typeinfo>

using namespace boost::numeric::ublas;

/// A wrapper class for gsl_multimin_fdfminimizer.
/**
 * This class is a wrapper class for gsl_multimin_fdfminimizer. 
 * It conforms to GSL version 1.11.
 */
class GSLFDFSolver : public NativeGradientSolver
{
 public:
  /// Defines the parameters of a GSLFDFSolver.
  struct Setup : public Cloneable< Setup, Solver::Setup >
  {
    double stepSize;  ///< initial step size for line searches
    double tol;       ///< tolerance for line search stopping criterion
    
    bool isCompatibleWith(const Solver &s) const;
    
    Setup(double stepSize, double tol) : stepSize(stepSize), tol(tol) { }
  };
  
  struct State : public Cloneable< State, NativeGradientSolver::State > { };
  
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
#ifdef WITH_LIBMATHEVAL
  GSLFDFSolver(const std::string &gslName,
               Function::DerivEvalType gEvalType = Function::DERIV_SYMBOLIC);
#else
  GSLFDFSolver(const std::string &gslName,
               Function::DerivEvalType gEvalType = Function::DERIV_FDIFF_CENTRAL_2);
#endif
  
  /// Constructs a new GSL solver with the given type.
  /**
   * Constructs a new GSL solver with the given type.
   * @param type see the GSL documentation
   * @param gEvalType gradient evaluator type to be used with this solver.
   */
#ifdef WITH_LIBMATHEVAL
  GSLFDFSolver(const gsl_multimin_fdfminimizer_type *type,
               Function::DerivEvalType gEvalType = Function::DERIV_SYMBOLIC);
#else
  GSLFDFSolver(const gsl_multimin_fdfminimizer_type *type,
               Function::DerivEvalType gEvalType = Function::DERIV_FDIFF_CENTRAL_2);
#endif
  
  ~GSLFDFSolver();
  
  double getFVal() const;
  const vector< double > getGradient() const;
  
  /// Returns the wrapped gsl_multimin_fdfminimizer object.
  const gsl_multimin_fdfminimizer *getGSLSolver() const;
  
  std::string getName() const;
  const State &getState() const { return state_; }
  const vector< double > getX() const;
  bool hasBuiltInStoppingCriterion() const { return false; }
  void restart();
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  gsl_multimin_function_fdf gslFunction_;
  gsl_multimin_fdfminimizer *gslSolver_;
  State state_;
  const gsl_multimin_fdfminimizer_type *type_;
  
  void doSetup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup = DefaultSetup(),
              const Constraints &C = NoConstraints());
  NativeSolver::IterationStatus iterate_();
};

#define GSLFDFSOLVER_H

#endif
