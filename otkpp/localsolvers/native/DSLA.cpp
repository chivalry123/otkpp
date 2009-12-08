
#include "DSLA.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

std::string DSLA::getName() const
{
  return "DSLA";
}

bool DSLA::hasBuiltInStoppingCriterion() const
{
  return true;
}

bool DSLA::isGSLSolver() const
{
  return false;
}

bool DSLA::usesGradient() const
{
  return false;
}

bool DSLA::usesHessian() const
{
  return false;
}

double DSLA::computeReduction_(const vector< double > &x,
                               const vector< double > &xPlus,
                               double f,
                               double fPlus,
                               const vector< double > &p) const
{
  double ared, pred;
  
  ared = f - fPlus;
  pred = -inner_prod(model_.getG(), p) - 
          0.5*inner_prod(p, prod(H_, p));
  
  return ared / pred;
}

double DSLA::computeTau_(const vector< double > &d,
                         const vector< double > &p)
{
  double pd = inner_prod(p, d);
  double d2 = inner_prod(d, d);
  double p2 = inner_prod(p, p);
  
  return (-pd + sqrt(pd*pd - d2*(p2 - delta_*delta_))) / d2;
}

vector< double > &DSLA::computeTrsRegStep_(const vector< double > &g,
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

NativeSolver::IterationStatus DSLA::iterate_()
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
  //bool lmStep = false;
  
  //H_ = zero_matrix< double >(n_, n_);
  computeTrsRegStep_(model_.getG(), H_, delta_, p_);
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
  
  /*vector< double > gOld(n_);
  objFunc_.g(x_, gOld);*/
  //vector< double > gOld = model_.getG();
  if(t != -1 && (fXPlus < model_.getLowestF() || inner_prod(p_, p_) <= maxDistSq))
    fImproved = model_.updatePoint(xPlus_, fXPlus, t);
  
  if(ratio > eta)
    delta_ *= 1.5;
  else
    delta_ *= 0.75;

  if(fImproved)
    model_.setOrigin(model_.getLowestIndex());
  
  /*if(fImproved)
  {*/
    double fXMinus = objFunc_(x_-p_);
    updateHessianApprox_(p_, fXPlus, fXMinus, f_);
  //}
  
  x_ = model_.getLowestX();
  f_ = model_.getLowestF();
  
  /*std::cout<<"i: "<<nIter_<<std::endl;
  model_.testInvariants();*/
  
  /*if(iterType_ == DSLA::GRADIENT)
    iterType_ = DSLA::MODEL;
  if(nIter_ % 2 == 0)
    iterType_ = DSLA::GRADIENT;*/
  
  /*if(!fImproved && nIter_ > 0)
    nIter_--;*/
  
  /*vector< double > gPlus(n_);
  objFunc_.g(x_, gPlus);
  if(fImproved)*/
    //HUpdater_.update(p_, gPlus-gOld/*model_.getG() - gOld*/, H_);
  /*if(nIter_ % 10 == 0)
    H_ = identity_matrix< double >(n_, n_);*/
  
  if(nIter_ < 10000 && delta_ > 1e-12)
    return NativeSolver::ITERATION_CONTINUE;
  else
    return NativeSolver::ITERATION_SUCCESS;
}

void DSLA::setup_(const Function &objFunc,
                  const vector< double > &x0,
                  const Solver::Setup &solverSetup,
                  const Constraints &C)
{
  NativeSolver::setup_(objFunc, x0, solverSetup);
  //HUpdater_ = BFGSUpdater(BFGSUpdater::DIRECT);
  
  delta_ = 1e-2;
  m_ = n_+1;
  
  x_ = x0;
  f_ = objFunc_(x0);
  
  model_ = LinInterp(objFunc, x0, delta_);
  model_.setOrigin(model_.getLowestIndex());
  p_.resize(n_);
  xPlus_.resize(n_);
  H_ = identity_matrix< double >(n_, n_);
}

void DSLA::updateHessianApprox_(const vector< double > &p,
                                double fxPlus,
                                double fxMinus,
                                double fx)
{
  vector< double > n = p / norm_2(p);
  double deltaF = fxPlus + fxMinus - 2.0*fx;
  double phi = deltaF / inner_prod(p, p);
  double phiBar = inner_prod(n, prod(H_, n));
  double epsBar = phi - phiBar;
  double alpha = pow(inner_prod(n, n), 2.0);
  double beta = epsBar / alpha;
  matrix< double > deltaH = beta * outer_prod(n, n);
  H_ += deltaH;
}
