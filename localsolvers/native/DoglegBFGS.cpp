
#include "BFGSUpdater.h"
#include "DoglegBFGS.h"
#include "DoglegSolver.h"
#include "SteihaugSolver.h"

#include <boost/numeric/ublas/io.hpp>

DoglegBFGS::DoglegBFGS()
{
  //lmatrixUpdater_ = new LBFGSUpdater(2, 5);
}

DoglegBFGS::~DoglegBFGS()
{
  
}

std::string DoglegBFGS::getName() const
{
  return "BFGS/DLG";
}

bool DoglegBFGS::isGSLSolver() const
{
  return false;
}

bool DoglegBFGS::usesGradient() const
{
  return true;
}

bool DoglegBFGS::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus DoglegBFGS::iterate_()
{
  bool nonzeroStep;
  bool r;
  
  /*if(nIter_ > 5)
  {*/
  /*r = lmatrixUpdater_->computeProduct(p_, q_, g_, Hg_);
    if(r == false)
      Hg_ = g_;*/
  /*}
  else
    Hg_ = g_;*/
  
  /*std::cout<<"Hg: "<<Hg_<<std::endl;
  std::cout<<"Hg(correct): "<<prod(H_,g_)<<std::endl;*/
  
  trSolver_.computeStep(x_, f_, g_, H_, p_, nonzeroStep, xPlus_, fPlus_);
  
  if(!nonzeroStep)
  {
    H_ = identity_matrix< double >(n_);
    return NativeSolver::ITERATION_SUCCESS;
  }
  
  objFunc_.g(xPlus_, gPlus_);
  q_ = gPlus_ - g_;
  
  matrixUpdater_.update(p_, q_, H_);
  //lmatrixUpdater_->updateVectors(p_, q_);
  
  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void DoglegBFGS::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const SolverSetup &solverSetup,
                        const Constraints &C)
{
  const int n = objFunc.getN();
  
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  
  matrixUpdater_ = BFGSUpdater(BFGSUpdater::DIRECT);
  trSolver_.setup(objFunc_);
  
  H_ = identity_matrix< double >(n);
  g_.resize(n);
  gPlus_.resize(n);
}
