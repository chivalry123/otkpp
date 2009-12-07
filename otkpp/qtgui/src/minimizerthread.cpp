
#include "BoundConstraints.h"
#include "GradNormTest.h"
#include "minimizerthread.h"
#include "NativeSolver.h"
#include "OTK.h"
#include "StoppingCriterion.h"

MinimizerThread::MinimizerThread(Function *objFunc,
                                 Solver *solver,
                                 double x0,
                                 double y0,
                                 const StoppingCriterion *stopCrit,
                                 const BoundConstraints *C) : 
  objFunc_(objFunc), solver_(solver), x0_(x0), y0_(y0),
  stopCrit_(stopCrit), terminationFlag_(false),
  C_(C) { }

MinimizerThread::~MinimizerThread()
{
  delete solver_;
  delete stopCrit_;
}

const MinimizerThread::Results &MinimizerThread::getResults() const
{
  return results_;
}

const Solver &MinimizerThread::getSolver()
{
  return *solver_;
}

void MinimizerThread::raiseTerminationFlag()
{
  terminationFlag_ = true;
}

void MinimizerThread::run()
{
  boost::numeric::ublas::vector< double > x0(2);
  x0(0) = x0_;
  x0(1) = y0_;
  
  if(!solver_->isExternalSolver())
  {
    NativeSolver *nativeSolver = dynamic_cast< NativeSolver * >(solver_);
    
    if(C_ != NULL)
      solver_->setup(*objFunc_, x0, Solver::DefaultSetup(), *C_);
    else
      solver_->setup(*objFunc_, x0, Solver::DefaultSetup(), NoConstraints());
    results_.iterates.clear();
    results_.iterates.push_back(x0);
    if(nativeSolver->getM() > 1)
    {
      results_.iteratePointSets.clear();
      results_.iteratePointSets.push_back(nativeSolver->getXArray());
    }
    
    unsigned int nIter = 0;
    NativeSolver::IterationStatus status = NativeSolver::ITERATION_CONTINUE;
    while(status == NativeSolver::ITERATION_CONTINUE && 
          (nativeSolver->hasBuiltInStoppingCriterion() || 
           !stopCrit_->test(*nativeSolver)) && 
          nIter < 10000 && !terminationFlag_)
    {
      status = nativeSolver->iterate();
      results_.iterates.push_back(nativeSolver->getX());
      if(nativeSolver->getM() > 1)
        results_.iteratePointSets.push_back(nativeSolver->getXArray());
      nIter++;
    }
    results_.xMin = nativeSolver->getX()[0];
    results_.yMin = nativeSolver->getX()[1];
    results_.fMin = nativeSolver->getFVal();
    results_.numIter = nativeSolver->getNumIter();
    results_.numFuncEval = nativeSolver->getNumFuncEval();
    results_.numGradEval = nativeSolver->getNumGradEval();
  }
  else
  {
    // TODO: stopping criterion for LMBM?
    boost::shared_ptr< Solver::Results > results = 
      solver_->solve(*objFunc_, x0, GradNormTest(1e-8));
    
    results_.xMin        = results->xMin[0];
    results_.yMin        = results->xMin[1];
    results_.fMin        = results->fMin;
    results_.numIter     = results->numIter;
    results_.numFuncEval = results->numFuncEval;
    results_.numGradEval = results->numGradEval;
    //results_.iterates    = results->iterates; // TODO: iterates?
  }
}
