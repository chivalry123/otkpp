
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
  /// Defines the parameters of a GSLFSolver.
  struct Setup : public Solver::Setup
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
  
  /// Returns the wrapper gsl_multimin_fminimizer object.
  const gsl_multimin_fminimizer *getGSLSolver() const;
  
  unsigned int getM() const;
  std::string getName() const;
  const vector< double > getX() const;
  const matrix< double > getXArray() const;
  bool hasBuiltInStoppingCriterion() const { return true; }
  bool isGSLSolver() const  { return true; }
  void restart();
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  gsl_multimin_function gslFunction_;
  gsl_multimin_fminimizer *gslSolver_;
  const gsl_multimin_fminimizer_type *type_;
  
  NativeSolver::IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup = DefaultSetup(),
              const Constraints &C = NoConstraints());
};

#define GSLFSOLVER_H

#endif
