
#include "NativeSolver.h"
#include "StoppingCriterion.h"

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <typeinfo>

static const unsigned int MAX_NUM_ITER = 50000;

/*NativeSolver::State *NativeSolver::State::clone() const
{
  return new NativeSolver::State(*this);
}*/

/*NativeSolver::NativeSolver(const NativeSolver &s) : baseState_(s.baseState_->clone()) { }

NativeSolver &NativeSolver::operator=(const NativeSolver &s)
{
  baseState_ = std::auto_ptr< NativeSolver::State >(s.baseState_->clone());
}*/

const vector< double > NativeSolver::getX() const
{
  return getState().x;
}

const matrix< double > NativeSolver::getXArray() const
{
  matrix< double > X(n_, 1);
  matrix_column< matrix< double > >(X, 0) = getState().x;
  
  return X;
}

double NativeSolver::getFVal() const
{
  return getState().f;
}

const vector< double > NativeSolver::getGradient() const
{
  throw std::runtime_error("the algorithm does not use gradient information");
}

const matrix< double > NativeSolver::getHessian() const
{
  throw std::runtime_error("the algorithm does not use Hessian information");
}

unsigned int NativeSolver::getNumIter() const
{
  return nIter_;
}

unsigned int NativeSolver::getNumFuncEval() const
{
  return objFunc_.getFuncEvalCounter();
}

unsigned int NativeSolver::getNumGradEval() const
{
  return objFunc_.getGradEvalCounter();
}

unsigned int NativeSolver::getNumHessEval() const
{
  return objFunc_.getHessEvalCounter();
}

NativeSolver::State &NativeSolver::getState_()
{
  return const_cast< NativeSolver::State & >(getState());
}

NativeSolver::IterationStatus NativeSolver::iterate()
{
  NativeSolver::IterationStatus status = iterate_();
  nIter_++;
  return status;
}

SolverResults NativeSolver::solve(const Function &objFunc,
                                  const vector< double > &x0,
                                  const Solver::Setup &solverSetup,
                                  const Constraints &C,
                                  const StoppingCriterion *stopCrit,
                                  bool timeTest)
{
  bool converged = false;
  std::list< vector< double > > iterates;
  unsigned int k = 0;
  SolverResults results;
  IterationStatus status = ITERATION_CONTINUE;
  
  if(stopCrit == NULL)
    throw std::invalid_argument("The solver requires a stopping criterion, but none given.");
  
  setup(objFunc, x0, solverSetup);
  
  objFunc_.resetEvalCounters();
  
  do
  {
    if(timeTest == false)
      iterates.push_back(getX());
    if(status == ITERATION_CONTINUE && converged)
      break;
    
    status = iterate();
    
    if(status != ITERATION_CONTINUE && !stopCrit->test(*this))
    {
      converged = false;
      break;
    }
    k++;
    
    converged = stopCrit->test(*this);
  }
  while(status == ITERATION_CONTINUE && 
        converged == false && k < MAX_NUM_ITER);
  
  results.converged   = converged;
  results.iterates    = iterates;
  results.xMin        = getX();
  results.fMin        = getFVal();
  results.numIter     = k;
  results.numFuncEval = getNumFuncEval();
  results.numGradEval = getNumGradEval();
  //results.numHessEval = getNumHessEval();
  results.time = 0; // TODO: write a special time test mode.
  
  return results;
}

/*void NativeSolver::allocateState_()
{
  state_ = new NativeSolver::State();
}*/

void NativeSolver::setup_(const Function &objFunc,
                          const vector< double > &x0,
                          const Solver::Setup &solverSetup,
                          const Constraints &C)
{
  getState_().x = x0;
  getState_().f = objFunc(x0);
  nIter_ = 0;
}
