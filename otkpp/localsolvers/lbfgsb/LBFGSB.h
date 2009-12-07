
#ifndef LBFGSB_H

#include <otkpp/localsolvers/native/GradientSolver.h>
#include <otkpp/constraints/BoundConstraints.h>

/// A wrapper class for Nocedal's L-BFGS-B Fortran implementation.
class LBFGSB : public GradientSolver
{
 public:
  /// Defines parameters for L-BFGS-B.
  struct Setup : public Solver::Setup
  {
    int m;
    
    /// Constructs a new L-BFGS-B setup.
    /**
     * @param m the length of iteration history.
     */
    Setup(int m);
    
    bool isCompatibleWith(const Solver &s) const;
  };
  
  struct State : public Cloneable< State, GradientSolver::State > { };
   
  /// Constructs a new L-BFGS-B solver with the given gradient evaluator type.
  LBFGSB(Function::DerivEvalType gEvalType = Function::DERIV_FDIFF_CENTRAL_2);

  const vector< double > getGradient() const;
  std::string getName() const;
  const State &getState() const { return state_; }
  bool hasBuiltInStoppingCriterion() const;
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
  std::vector< int > iwa_;
  int m_;
  std::vector< int > nbd_;
  double pgtol_;
  State state_;
  char task_[60];
  std::vector< double > wa_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const Solver::Setup &solverSetup,
              const Constraints &C);
};

#define LBFGSB_H

#endif
