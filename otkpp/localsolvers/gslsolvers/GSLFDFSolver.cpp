
#include "GSLFDFSolver.h"
#include "GSLUtils.h"

bool GSLFDFSolver::Setup::isCompatibleWith(const Solver &s) const
{
  if(typeid(s) == typeid(GSLFDFSolver))
    return true;
  else
    return false;
}

GSLFDFSolver::GSLFDFSolver(const std::string &gslName, Function::DerivEvalType gEvalType) : 
  GradientSolver(gEvalType)
{
  if(gslName == "conjugate_fr")
    type_ = gsl_multimin_fdfminimizer_conjugate_fr;
  else if(gslName == "conjugate_pr")
    type_ = gsl_multimin_fdfminimizer_conjugate_pr;
  else if(gslName == "vector_bfgs")
    type_ = gsl_multimin_fdfminimizer_vector_bfgs;
  else if(gslName == "vector_bfgs2")
    type_ = gsl_multimin_fdfminimizer_vector_bfgs2;
  else if(gslName == "steepest_descent")
    type_ = gsl_multimin_fdfminimizer_steepest_descent;
  else
    throw std::invalid_argument("invalid GSL minimizer name");
  
  gslSolver_ = NULL;
}

GSLFDFSolver::GSLFDFSolver(const gsl_multimin_fdfminimizer_type *type, Function::DerivEvalType gEvalType) : 
  GradientSolver(gEvalType)
{
  type_ = type;
  gslSolver_ = NULL;
}

GSLFDFSolver::~GSLFDFSolver()
{
  if(gslSolver_ != NULL)
  {
    free(gslSolver_->x->block);
    free(gslSolver_->gradient->block);
    gsl_multimin_fdfminimizer_free(gslSolver_);
  }
}

double GSLFDFSolver::getFVal() const
{
  return gslSolver_->f;
}

const vector< double > GSLFDFSolver::getGradient() const
{
  vector< double > g;
  gslutils::GSLToBoostVector(gslSolver_->gradient, g);
  return g;
}

const gsl_multimin_fdfminimizer *GSLFDFSolver::getGSLSolver() const
{
  return gslSolver_;
}

std::string GSLFDFSolver::getName() const
{
  return type_->name;
}

const vector< double > GSLFDFSolver::getX() const
{
  vector< double > x;
  gslutils::GSLToBoostVector(gslSolver_->x, x);
  return x;
}

NativeSolver::IterationStatus GSLFDFSolver::iterate_()
{
  int status = gsl_multimin_fdfminimizer_iterate(gslSolver_);
  state_.fx = gslSolver_->f;
  
  // TODO: error codes
  if(status == GSL_ENOPROG)
    return NativeSolver::ITERATION_NO_PROGRESS;
  else
    return NativeSolver::ITERATION_CONTINUE;
}

void GSLFDFSolver::doSetup_(const Function &objFunc,
                            const vector< double > &x0,
                            const Solver::Setup &solverSetup,
                            const Constraints &C)
{
  const int n = objFunc.getN();
  double stepSize, tol;
  
  GradientSolver::doSetup_(objFunc, x0, solverSetup, C);
  
  if(typeid(solverSetup) == typeid(Solver::DefaultSetup))
  {
    stepSize = 1.0;
    tol = 0.1;
  }
  else
  {
    stepSize = dynamic_cast< const GSLFDFSolver::Setup & >(solverSetup).stepSize;
    tol = dynamic_cast< const GSLFDFSolver::Setup & >(solverSetup).tol;
  }
  
  if(gslSolver_ != NULL)
  {
    free(gslSolver_->x->block);
    free(gslSolver_->gradient->block);
    gsl_multimin_fdfminimizer_free(gslSolver_);
  }
  gslSolver_ = gsl_multimin_fdfminimizer_alloc(type_, n);
  
  // THIS IS A HACK!
  free(gslSolver_->x->block->data);
  gslSolver_->x->data = gslSolver_->x->block->data = &state_.x[0];
  gslSolver_->x->owner = 0;
  gslSolver_->x->size = n;
  gslSolver_->x->stride = 1;
  free(gslSolver_->gradient->block->data);
  gslSolver_->gradient->data = gslSolver_->gradient->block->data = &state_.g[0];
  gslSolver_->gradient->owner = 0;
  gslSolver_->gradient->size = n;
  gslSolver_->gradient->stride = 1;
  // !!!!!!!!!!!!!!!
  
  gslFunction_.n      = n;
  gslFunction_.params = NULL;
  gslFunction_.f      = &gslutils::f;
  // by using a pointer-to-member it would be something like this:
  // (double (*)(const gsl_vector *, void *))(gslWrapper_.*(&GSLWrapper::f));
  gslFunction_.df     = &gslutils::df;
  gslFunction_.fdf    = &gslutils::fdf;
  
  gslutils::setFunction(setup_->f);
  gsl_vector *x0_ = gsl_vector_alloc(n);
  gslutils::BoostToGSLVector(x0, x0_);
  gsl_multimin_fdfminimizer_set(gslSolver_, &gslFunction_, x0_, stepSize, tol);
  gsl_vector_free(x0_);
}

bool GSLFDFSolver::usesGradient() const
{
  return true;
}

bool GSLFDFSolver::usesHessian() const
{
  return false;
}
