
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
                               const vector< double > &p) const
{
  double ared, pred;
  
  ared = objFunc_(x) - objFunc_(xPlus);
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

vector< double > &DSQA::computeTrsRegStep2_(const vector< double > &g,
                                           const matrix< double > &H,
                                           double delta,
                                           vector< double > &p)
{
  double tau;
  
  if(inner_prod(g, prod(H, g)) <= 0.0)
    tau = 1.0;
  else
    tau = std::min(pow(norm_2(g), 3.0) / 
                   (delta * inner_prod(g, prod(H, g))), 1.0);
  
  p = -tau * delta / norm_2(g) * g;
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
  //double rho;
  vector< double > pPlus;
  int j, k;
  double r0_norm = norm_2(g);
  vector< double > Hd;
  double rr;
  double rPlus2;
  
  double eps = 1e-16;
  
  j = 0;
  /*do
  {*/
    if(r0_norm < eps)
    {
      //std::cout<<"CG termination 4"<<std::endl;
      p = zero_vector< double >(n_);
      return p;
    }
    
    r = g;
    d = -r;
    p = zero_vector< double >(n_);
    
    k = 0;
    while(k < n_)
    {
      Hd = prod(H, d);
      dHd = inner_prod(d, Hd);
      if(dHd <= 0.0)
      {
        //std::cout<<"CG termination 1"<<std::endl;
        tau = computeTau_(d, p);
        pPlus = p + tau*d;
        break;
      }
      rr = inner_prod(r, r);
      alpha = rr / dHd;
      pPlus = p + alpha*d;
      if(norm_2(pPlus) >= delta)
      {
        //std::cout<<"CG termination 2"<<std::endl;
        tau = computeTau_(d, p);
        pPlus = p + tau*d;
        break;
      }
      rPlus = r + alpha * Hd;
      rPlus2 = inner_prod(rPlus, rPlus);
      if(rPlus2 < (eps*r0_norm)*(eps*r0_norm))
      {
        //std::cout<<"CG termination 3"<<std::endl;
        break;
      }
      beta = rPlus2 / rr;
      d = -rPlus + beta*d;
      r = rPlus;
      p = pPlus;
      k++;
    }
    
    //end:
    p = pPlus;
    /*std::cout<<"n. CG iterations: "<<k<<std::endl;
    std::cout<<"p norm: "<<norm_2(p)<<std::endl;
    std::cout<<"CG residual: "<<r<<std::endl;*/
    /*if(inner_prod(p, p) > 0.25*rho_*rho_)
    {
      p = pPlus;
      xPlus = x_ + p;
      fxPlus = objFunc_(xPlus);
      
      ratio = computeReduction_(x_, xPlus, p);
      if(ratio > 0.1)
        continueIter = false;
    }
    else
      rho_ *= 0.2;*/
    
    j++;
  /*}
  while(continueIter && j < 100);*/
  
  /*p = pPlus;
  xPlus = x_ + p;
  fxPlus = objFunc_(xPlus);*/
  
  /*if(j == 100)
  {
    p = zero_vector< double >(n_);
    xPlus = x_;
    fxPlus = f_;
  }*/
  
  return p;
}

NativeSolver::IterationStatus DSQA::iterate_()
{
  double fXPlus;
  double distSq;
  vector< double > dx(n_);
  int j;
  double maxDistSq = 0.0;
  vector< double > p(n_);
  double ratio;
  int t = 0;
  bool trsRegIter = true;
  vector< double > xPlus(n_);
  double eta = 0.0;

  computeTrsRegStep_(model_.getG(), model_.getH(), delta_, p);
  xPlus = x_ + p;
  fXPlus = objFunc_(xPlus);
  ratio = computeReduction_(x_, xPlus, p);

  /*std::cout<<"delta: "<<delta_<<std::endl;
  std::cout<<"ratio: "<<ratio<<std::endl;*/
  /*std::cout<<"p: "<<p<<std::endl;
  std::cout<<"fx_k: "<<f_<<std::endl;
  std::cout<<"fx_best: "<<fXBest_<<std::endl;
  std::cout<<"fxplus: "<<fXPlus<<std::endl;
  std::cout<<"Q(xk): "<<c_<<std::endl;
  std::cout<<"H: "<<H_<<std::endl;
  std::cout<<"g: "<<g_<<std::endl;
  std::cout<<"c: "<<c_<<std::endl;
  for(int i = 0; i < m_; i++)
    std::cout<<"x["<<i+1<<"]: "<<X_[i]<<std::endl;
//testLagrange_();
  if(nIter_ == 20)
    throw std::runtime_error("...");
  if(fXPlus > f_ && ratio > 0.0)
    throw std::runtime_error("trust region step failed");*/

/*if(c_+ inner_prod(g_, p) + 0.5*inner_prod(p, prod(H_, p)) > c_)
  throw std::runtime_error("...");*/
  
  t = -1;
  maxDistSq = 0.0;
  for(j = 0; j < m_; j++)
  {
    dx = model_.getLowestX() - model_.getX()[j];
    distSq = inner_prod(dx, dx);
    if(distSq > maxDistSq)
    {
      maxDistSq = distSq;
      t = j;
    }
  }
  
  if(fXPlus < model_.getLowestF() || 
      inner_prod(xPlus-model_.getLowestX(), xPlus-model_.getLowestX()) <= maxDistSq)
    model_.updatePoint(xPlus, t);
    
  if(ratio > eta)
    delta_ *= 1.5;
  else
    delta_ *= 0.75;

  model_.setOrigin(model_.getLowestX());
  
  x_ = model_.getLowestX();
  f_ = model_.getLowestF();
  
  //model_.testInvariants();
  
  if(nIter_ < 1000 && delta_ > 1e-6)
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
  //trsRegMinimizer_.setup(objFunc);
  
  delta_ = 0.01;
  
  m_ = (n_+1)*(n_+2)/2;
  
  x_ = x0;
  f_ = objFunc_(x0);
  
  model_ = QuadInterp(objFunc, x0, delta_);
  
  /*for(int i = 0; i < m_; i++)
    std::cout<<X_[i][0]<<" "<<X_[i][1]<<std::endl;
  matrix< double > H(n_, n_);
  objFunc_.H(x0, H);
  std::cout<<H<<std::endl;
  std::cout<<H_<<std::endl;*/
  
  /*testLagrange_();
  throw std::runtime_error("test completed");*/
}
