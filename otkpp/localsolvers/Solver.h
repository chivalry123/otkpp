
#ifndef SOLVER_H

#include <otkpp/constraints/Constraints.h>
#include <otkpp/function/Function.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::numeric::ublas;

class Function;
class Solver;
class StoppingCriterion;

class Solver
{
 public:
  /// Defines the parameters of a solver.
  struct Setup
  {
    boost::shared_ptr< Constraints > C;
    unsigned int m;
    unsigned int n;
    Function objFunc;
    
    virtual Setup *clone() const = 0;
    
    /// Is this solver setup compatible with the given solver.
    virtual bool isCompatibleWith(const Solver &s) const = 0;
  };

  /// Defines a default solver setup specifying that default parameters are used.
  struct DefaultSetup : public Cloneable< DefaultSetup, Setup >
  {
    bool isCompatibleWith(const Solver &s) const { return true; }
  };
  
  /// Defines the results of a solver.
  /**
   * This class defines the results produced by 
   * iteration of a solver.
   */
  struct Results
  {
    bool converged;                         ///< was the chosen stopping criterion satisfied
    double fMin;                            ///< final function value
    //std::list< vector< double > > iterates; ///< iteration points for each iteration step
    unsigned int numFuncEval;               ///< the number of used function evaluations
    unsigned int numGradEval;               ///< the number of used gradient evaluations
    unsigned int numIter;                   ///< the number of used iterations
    boost::shared_ptr< Setup > setup;       ///< the used input parameters
    double termVal;                         ///< the final termination test value
    double time;                            ///< used time
    vector< double > xMin;                  ///< the final iterate
    
    virtual ~Results() { };
  };
  
  virtual ~Solver() { }
  
  /// Returns the number of points this solver produces each iteration (default = 1).
  virtual unsigned int getM() const { return 1; }
  
  /// Returns the number of dimensions of the problem associated with this solver.
  unsigned int getN() const { return n_; }
  
  /// Returns the name of this solver.
  virtual std::string getName() const = 0;
  
  /// Is this solver a native or third-party implementation.
  virtual bool isExternalSolver() const { return false; }
  
  /// Initializes this solver.
  /**
   * Initializes this solver with objective 
   * function objFunc and starting point x0. 
   * The dimensions of objFunc and x0 must match.
   * @param objFunc objective function
   * @param x0 starting point
   * @param solverSetup a structure containing algorithm-specific parameters
   */
  void setup(const Function &objFunc,
             const vector< double > &x0,
             const Setup &solverSetup = DefaultSetup(),
             const Constraints &C = NoConstraints());
  
  /// Solves the given problem by using this solver and returns the results.
  /**
   * @param objFunc objective function
   * @param x0 starting point
   * @param solverSetup solver parameters
   * @param C constraints
   * @param stopCrit stopping criterion
   * @return a SolverResults structure containing the results
   */
  virtual boost::shared_ptr< Solver::Results > solve(Function &objFunc,
                                                     const vector< double > &x0,
                                                     const StoppingCriterion &stopCrit,
                                                     const Setup &solverSetup = DefaultSetup(),
                                                     const Constraints &C = NoConstraints(),
                                                     bool timeTest = false) = 0;
  
  /// Does this solver support the given constraints.
  virtual bool supportsConstraints(const Constraints &C) { return false; }
  
  /// Does this solver use gradient information.
  virtual bool usesGradient() const = 0;
  
  /// Does this solver use Hessian information.
  virtual bool usesHessian() const = 0;
 protected:
  int n_;
  Function objFunc_;
  
  virtual void setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const Setup &solverSetup,
                      const Constraints &C) = 0;
};

#define SOLVER_H

#endif
