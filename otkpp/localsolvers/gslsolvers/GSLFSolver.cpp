
#include "GSLFSolver.h"
#include "GSLUtils.h"

typedef struct
{
  gsl_matrix *x1;
  gsl_vector *y1;
  gsl_vector *ws1;
  gsl_vector *ws2;
}
nmsimplex_state_t;

bool GSLFSolver::Setup::isCompatibleWith(const Solver &s) const
{
  if(typeid(s) == typeid(GSLFSolver))
    return true;
  else
    return false;
}

GSLFSolver::GSLFSolver(const std::string &gslName/*,
                       const vector< double > &stepSize*/)
{
  if(gslName == "nmsimplex")
    type_ = gsl_multimin_fminimizer_nmsimplex;
  else
    throw std::invalid_argument("invalid GSL minimizer name");
  
  gslSolver_ = NULL;
}

GSLFSolver::GSLFSolver(const gsl_multimin_fminimizer_type *type)
{
  type_ = type;
  gslSolver_ = NULL;
}

GSLFSolver::~GSLFSolver()
{
  if(gslSolver_ != NULL)
  {
    free(gslSolver_->x->block);
    gsl_multimin_fminimizer_free(gslSolver_);
  }
}

double GSLFSolver::getFVal() const
{
  return gslSolver_->fval;
}

const gsl_multimin_fminimizer *GSLFSolver::getGSLSolver() const
{
  return gslSolver_;
}

unsigned int GSLFSolver::getM() const
{
  return n_ + 1;
}

std::string GSLFSolver::getName() const
{
  return type_->name;
}

const vector< double > GSLFSolver::getX() const
{
  vector< double > x;
  gslutils::GSLToBoostVector(gslSolver_->x, x);
  return x;
}

const matrix< double > GSLFSolver::getXArray() const
{
  matrix< double > X(n_, n_ + 1);
  
  nmsimplex_state_t *state = (nmsimplex_state_t *)gslSolver_->state;
  
  for(int i = 0; i < n_; i++)
    for(int j = 0; j < n_ + 1; j++)
      X(i, j) = gsl_matrix_get(state->x1, j, i);
  
  return X;
}

NativeSolver::IterationStatus GSLFSolver::iterate_()
{
  int status = gsl_multimin_fminimizer_iterate(gslSolver_);
  state_.f = gslSolver_->fval;
  
  if(gsl_multimin_test_size(gslSolver_->size, 1e-12) == GSL_SUCCESS)
    return NativeSolver::ITERATION_SUCCESS;
  
  if(status == GSL_ENOPROG)
    return NativeSolver::ITERATION_NO_PROGRESS;
  else
    return NativeSolver::ITERATION_CONTINUE;
}

void GSLFSolver::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const Solver::Setup &solverSetup,
                        const Constraints &C)
{
  const int n = objFunc.getN();
  vector< double > stepSize;
  
  NativeSolver::setup_(objFunc, x0, solverSetup);
  
  if(typeid(solverSetup) == typeid(Solver::DefaultSetup))
  {
    stepSize = vector< double >(n);
    for(int i = 0; i < n; i++)
      stepSize(i) = 0.9;
  }
  else
    stepSize = dynamic_cast< const GSLFSolver::Setup & >(solverSetup).stepSize;
  
  if(gslSolver_ != NULL)
  {
    free(gslSolver_->x->block);
    gsl_multimin_fminimizer_free(gslSolver_);
  }
  gslSolver_ = gsl_multimin_fminimizer_alloc(type_, n);
  
  // THIS IS A HACK!
  free(gslSolver_->x->block->data);
  gslSolver_->x->data = gslSolver_->x->block->data = &state_.x[0];
  gslSolver_->x->owner = 0;
  gslSolver_->x->size = gslSolver_->x->block->size = n;
  gslSolver_->x->stride = 1;
  // !!!!!!!!!!!!!!!
  
  gslFunction_.n = n;
  gslFunction_.params = NULL;
  gslFunction_.f = &gslutils::f;
  
  gslutils::setFunction(objFunc);
  gsl_vector *x0_ = gsl_vector_alloc(n);
  gslutils::BoostToGSLVector(x0, x0_);
  
  gsl_vector *step_size = gsl_vector_alloc(stepSize.size());
  gslutils::BoostToGSLVector(stepSize, step_size);
  gsl_multimin_fminimizer_set(gslSolver_, &gslFunction_, x0_, step_size);
  gsl_vector_free(step_size);
  gsl_vector_free(x0_);
}

bool GSLFSolver::usesGradient() const
{
  return false;
}

bool GSLFSolver::usesHessian() const
{
  return false;
}
