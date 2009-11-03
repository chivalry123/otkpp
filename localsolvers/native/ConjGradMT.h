
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
    FLETCHER_REEVES, ///< Fletcher-Reeves
    POLAK_RIBIERE    ///< Polak-Ribiere
  };
  
  /// Constructs a new conjugate gradient solver of the given type.
  ConjGradMT(Type type);
  
  std::string getName() const;
  bool isGSLSolver() const;
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  int c_;
  vector< double > d_;
  double deltaF_;
  int i_;
  MoreThuente lineMinimizer_;
  Type type_;
  
  IterationStatus iterate_();
  void setup_(const Function &objFunc,
              const vector< double > &x0,
              const SolverSetup &solverSetup,
              const Constraints &C);
};

#define CONJGRADMT_H

#endif
