
#include "DSQA.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

std::string DSQA::getName() const
{
  return "DSQA";
}

bool DSQA::hasBuiltInStoppingCriterion() const
{
  return true;
}

bool DSQA::isGSLSolver() const
{
  return false;
}

bool DSQA::usesGradient() const
{
  return false;
}

bool DSQA::usesHessian() const
{
  return false;
}

double DSQA::computeReduction_(const vector< double > &x,
                               const vector< double > &xPlus,
                               double f,
                               double fPlus,
                               const vector< double > &p) const
{
  double ared, pred;
  
  ared = f - fPlus;
  pred = -inner_prod(model_.getG(), p) - 
          0.5*inner_prod(p, prod(model_.getH(), p));
  
  return ared / pred;
}

double DSQA::computeTau_(const vector< double > &d,
                         const vector< double > &p)
{
  double pd = inner_prod(p, d);
  double d2 = inner_prod(d, d);
  double p2 = inner_prod(p, p);
  
  return (-pd + sqrt(pd*pd - d2*(p2 - delta_*delta_))) / d2;
}

vector< double > &DSQA::computeTrsRegStep_(const vector< double > &g,
                                           const matrix< double > &H,
                                           double delta,
                                           vector< double > &p)
{
  vector< double > r;
  vector< double > rPlus;
  vector< double > d;
  double dHd;
  double alpha, beta;
  double tau;
  vector< double > pPlus;
  int k;
  double r0_norm = norm_2(g);
  vector< double > Hd;
  double rr;
  double rPlus2;
  
  double eps = 1e-16;
  
  noalias(p) = zero_vector< double >(n_);
  
  if(r0_norm < eps)
  {
    p = zero_vector< double >(n_);
    return p;
  }
  
  r = g;
  d = -r;
  
  k = 0;
  while(k < n_)
  {
    Hd = prod(H, d);
    dHd = inner_prod(d, Hd);
    if(dHd <= 0.0)
    {
      tau = computeTau_(d, p);
      pPlus = p + tau*d;
      break;
    }
    rr = inner_prod(r, r);
    alpha = rr / dHd;
    pPlus = p + alpha*d;
    if(norm_2(pPlus) >= delta)
    {
      tau = computeTau_(d, p);
      pPlus = p + tau*d;
      break;
    }
    rPlus = r + alpha * Hd;
    rPlus2 = inner_prod(rPlus, rPlus);
    if(rPlus2 < (eps*r0_norm)*(eps*r0_norm))
      break;
    beta = rPlus2 / rr;
    d = -rPlus + beta*d;
    r = rPlus;
    p = pPlus;
    k++;
  }
    
  p = pPlus;
  
  return p;
}

NativeSolver::IterationStatus DSQA::iterate_()
{
  double fXPlus;
  double distSq;
  vector< double > dx(n_);
  bool fImproved = false;
  int j;
  double maxDistSq = 0.0;
  double ratio;
  int t = 0;
  bool trsRegIter = true;
  double eta = 0.0;

  computeTrsRegStep_(model_.getG(), model_.getH(), delta_, p_);
  xPlus_ = x_ + p_;
  fXPlus = objFunc_(xPlus_);
  ratio = computeReduction_(x_, xPlus_, f_, fXPlus, p_);

  t = -1;
  maxDistSq = 0.0;
  for(j = 0; j < m_; j++)
  {
    if(j == model_.getLowestIndex())
      continue;
    dx = model_.getLowestX() - model_.getX()[j];
    distSq = inner_prod(dx, dx);
    if(distSq > maxDistSq)
    {
      maxDistSq = distSq;
      t = j;
    }
  }
  
  if(fXPlus < model_.getLowestF() || inner_prod(p_, p_) <= maxDistSq)
     fImproved = model_.updatePoint(xPlus_, fXPlus, t);
    
  if(ratio > eta)
    delta_ *= 1.5;
  else
    delta_ *= 0.75;

  if(fImproved)
    model_.setOrigin(model_.getLowestX());
  
  x_ = model_.getLowestX();
  f_ = model_.getLowestF();
  
  /*std::cout<<"i: "<<nIter_<<std::endl;
  model_.testInvariants();*/
  
  if(nIter_ < 10000 && delta_ > 1e-12)
    return NativeSolver::ITERATION_CONTINUE;
  else
    return NativeSolver::ITERATION_SUCCESS;
}

void DSQA::setup_(const Function &objFunc,
                  const vector< double > &x0,
                  const SolverSetup &solverSetup,
                  const Constraints &C)
{
  NativeSolver::setup_(objFunc, x0, solverSetup);
  
  delta_ = 1e-3;
  m_ = (n_+1)*(n_+2)/2;
  
  x_ = x0;
  f_ = objFunc_(x0);
  
  model_ = QuadInterp(objFunc, x0, delta_);
  model_.setOrigin(model_.getLowestX());
  p_.resize(n_);
  xPlus_.resize(n_);
}
