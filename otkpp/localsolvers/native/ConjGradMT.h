
#ifndef CONJGRADMT_H

#include <otkpp/localsolvers/native/GradientSolver.h>
#include <otkpp/localsolvers/native/linmin/MoreThuente.h>

class MoreThuente;

/// Implements conjugate gradient algorithms.
/**
 * Implements the Fletcher-Reeves and Polak-Ribiere 
 * conjugate gradient algorithms with the More and 
 * Thuente line search.
 */
class ConjGradMT : public GradientSolver
{
 public:
  /// Defines the type of the algorithm.
  enum Type
  {
    FLETCHER_REEVES,
    POLAK_RIBIERE
  };
  
  struct State : public Cloneable< State, GradientSolver::State >
  {
    vector< double > d;
  };
  
  /// Constructs a new conjugate gradient solver of the given type.
  ConjGradMT(Type type);
  
  std::string getName() const;
  const State &getState() const { return state_; }
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  int c_;
  double deltaF_;
  int i_;
  MoreThuente lineMinimizer_;
  State state_;
  Type type_;
  
  IterationStatus iterate_();
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C);
};

#define CONJGRADMT_H

#endif
