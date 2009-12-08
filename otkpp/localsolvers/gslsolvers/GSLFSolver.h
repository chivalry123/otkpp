
#ifndef GSLFSOLVER_H

#include <otkpp/localsolvers/gslsolvers/GSLUtils.h>
#include <otkpp/localsolvers/native/NativeSolver.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <gsl/gsl_multimin.h>
#include <typeinfo>

using namespace boost::numeric::ublas;

/// A wrapper class for gsl_multimin_fminimizer.
/**
 * This class is a wrapper class for gsl_multimin_fminimizer. 
 * It conforms to GSL version 1.11.
 */
class GSLFSolver : public NativeSolver
{
 public:
  struct State : public Cloneable< State, NativeSolver::State > { };
  
  /// Defines the parameters of a GSLFSolver.
  struct Setup : public Cloneable< Setup, Solver::Setup >
  {
    vector< double > stepSize;  ///< initial simplex size
    
    bool isCompatibleWith(const Solver &s) const;
    Setup(const vector< double > &stepSize) : stepSize(stepSize) { }
  };

  /// Constructs a new GSL solver with the given name.
  /**
   * Constructs a new GSL solver with the given name. 
   * Currently only "nmsimplex" is supported.
   */
  GSLFSolver(const std::string &gslName);
  
  GSLFSolver(const gsl_multimin_fminimizer_type *type);
  ~GSLFSolver();
  
  double getFVal() const;
  
  /// Returns the wrapped gsl_multimin_fminimizer object.
  const gsl_multimin_fminimizer *getGSLSolver() const;
  
  unsigned int getM() const;
  std::string getName() const;
  const State &getState() const { return state_; }
  const vector< double > getX() const;
  const matrix< double > getXArray() const;
  bool hasBuiltInStoppingCriterion() const { return true; }
  void restart();
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  gsl_multimin_function gslFunction_;
  gsl_multimin_fminimizer *gslSolver_;
  State state_;
  const gsl_multimin_fminimizer_type *type_;
  
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup = DefaultSetup(),
                const Constraints &C = NoConstraints());
  NativeSolver::IterationStatus iterate_();
};

#define GSLFSOLVER_H

#endif
