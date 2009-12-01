
#include "cholesky.hpp"
#include "MNewton.h"
#include "MoreThuente.h"

#include <boost/numeric/ublas/triangular.hpp>

MNewton::MNewton()
{
  lineMinimizer_ = new MoreThuente();
}

MNewton::~MNewton()
{
  delete lineMinimizer_;
}

std::string MNewton::getName() const
{
  return "MNewton";
}

bool MNewton::isGSLSolver() const
{
  return false;
}

bool MNewton::usesGradient() const
{
  return true;
}

bool MNewton::usesHessian() const
{
  return true;
}

NativeSolver::IterationStatus MNewton::iterate_()
{
  double alpha;
  
  objFunc_.H(x_, H_);
  d_ = -g_;
  int r = cholesky_decompose(H_, H_chol_);
  /*while(r != 0)
  {
    H_ = H_ + 1e-3 * identity_matrix< double >(n_);
    r = cholesky_decompose(H_, H_chol_);
  }*/
  inplace_solve(H_chol_, d_, lower_tag());
  inplace_solve(trans(H_chol_), d_, upper_tag());
  
  lineMinimizer_->minimize(x_, d_, 1.0, f_, g_,
                           alpha, xPlus_, fPlus_, gPlus_);
  
  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void MNewton::setup_(const Function &objFunc,
                     const vector< double > &x0,
                     const Solver::Setup &solverSetup,
                     const Constraints &C)
{
  const int N = objFunc.getN();
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  lineMinimizer_->setup(objFunc_, MoreThuente::Setup());
  H_.resize(N, N);
  H_chol_.resize(N, N);
}
