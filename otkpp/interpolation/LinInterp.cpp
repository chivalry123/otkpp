
#include "LinInterp.h"

#include <boost/numeric/ublas/io.hpp>

LinInterp::LinInterp() : n_(0), m_(0) { }

LinInterp::LinInterp(const Function &f, const vector< double > &xb, double delta)
{
  int i;
  
  f_ = f;
  n_ = f.getN();
  m_ = n_+1;
  
  X_.resize(m_);
  for(i = 0; i < m_; i++)
    X_[i].resize(n_);
  F_.resize(m_);
  
  g_.resize(n_);
  
  cl_.resize(m_);
  gl_.resize(m_);
  for(i = 0; i < m_; i++)
    gl_[i].resize(n_);
  
  initialize_(xb, delta);
}

double LinInterp::eval(const vector< double > &d) const
{
  return c_ + inner_prod(g_, d);
}

double LinInterp::evalLagrangian(int j, const vector< double > &d) const
{
  return cl_[j] + inner_prod(gl_[j], d);
}

const vector< double > &LinInterp::getG() const
{
  return g_;
}

double LinInterp::getLowestF() const
{
  return F_[xiLowest_];
}

int LinInterp::getLowestIndex() const
{
  return xiLowest_;
}

const vector< double > &LinInterp::getLowestX() const
{
  return X_[xiLowest_];
}

const std::vector< vector< double > > &LinInterp::getX() const
{
  return X_;
}

void LinInterp::setOrigin(int xbi)
{
  const vector< double > &xb = X_[xbi];
  vector< double > dx = xb - xb_;
  
  for(int i = 0; i < m_; i++)
    cl_[i] += inner_prod(dx, gl_[i]);
  
  c_ += inner_prod(dx, g_);
  
  xb_ = xb;
  
  testInvariants();
}

void LinInterp::testInvariants()
{
  return;
  int i;
  double lix, Lx = 0.0;
  vector< double > x;
  
  for(i = 0; i < m_; i++)
  {
    if(f_(X_[i]) != F_[i])
      throw std::runtime_error("invalid function value");
    if(f_(X_[i]) < F_[xiLowest_])
      throw std::runtime_error("invalid best point");
    if(fabs(eval(X_[i] - xb_) - F_[i]) > 1e-3)
    {
      std::cout<<"Q(x): "<<eval(X_[i] - xb_)<<std::endl;
      std::cout<<"f(x): "<<F_[i]<<std::endl;
      throw std::runtime_error("invalid interpolation");
    }
  }
  
  x = xb_;
  for(i = 0; i < n_; i++)
    x[i] += (rand() % 100) / 100.0;
  for(i = 0; i < m_; i++)
  {
    lix = evalLagrangian(i, x-xb_);
    Lx += F_[i] * lix;
  }
  if(fabs(Lx - eval(x-xb_)) > 1e-3)
  {
    std::cout<<"L(x): "<<Lx<<std::endl;
    std::cout<<"Q(x): "<<eval(x-xb_)<<std::endl;
    throw std::runtime_error("mismatching lagrange functions and quadratic model");
  }
}

bool LinInterp::updatePoint(const vector< double > &x, int j)
{
  return updatePoint(x, NAN, j);
}

bool LinInterp::updatePoint(const vector< double > &x, double fx, int j)
{
  // Update the Lagrange coefficients.
  double c1, c2;
  vector< double > dx = x - xb_;
  int i;
  bool improved = false;
  double m;
  
  if(isnan(fx))
    fx = f_(x);
  
  c2 = evalLagrangian(j, dx);
  
  cl_[j] /= c2;
  gl_[j] /= c2;
  
  for(i = 0; i < m_; i++)
  {
    if(i == j)
      continue;
    
    c1 = evalLagrangian(i, dx);
    
    cl_[i] -= c1*cl_[j];
    gl_[i] -= c1*gl_[j];
  }
  
  if(fx < F_[xiLowest_])
  {
    xiLowest_ = j;
    improved = true;
  }
  
  // Update the linear model.
  m = fx - eval(dx);
  c_ += m*cl_[j];
  g_ += m*gl_[j];
  
  X_[j] = x;
  F_[j] = fx;
  
  testInvariants();
  
  return improved;
}

void LinInterp::initialize_(const vector< double > &xb, double delta)
{
  double fxb = f_(xb);
  int i, j;
  
  xb_ = xb;
  
  X_[0] = xb;
  F_[0] = fxb;
  for(i = 1; i < m_; i++)
  {
    X_[i] = xb;
    X_[i][i-1] += delta;
    F_[i] = f_(X_[i]);
  }
  
  c_ = fxb;
  for(i = 0; i < n_; i++)
    g_[i] = (F_[i+1] - fxb) / delta;
  
  for(i = 1; i < m_; i++)
  {
    cl_[i] = 0.0;
    for(j = 0; j < n_; j++)
    {
      if(j == i-1)
        gl_[i][j] = 1.0 / delta;
      else
        gl_[i][j] = 0.0;
    }
  }
  
  cl_[0] = 1.0;
  gl_[0] = zero_vector< double >(n_);
  for(i = 1; i < m_; i++)
  {
    cl_[0] -= cl_[i];
    gl_[0] -= gl_[i];
  }
  
  /*std::cout<<"c: "<<c_<<std::endl;
  std::cout<<"g: "<<g_<<std::endl;
  for(i = 0; i < m_; i++)
  {
    std::cout<<"X["<<i<<"]: "<<X_[i]<<std::endl;
    std::cout<<"cl["<<i<<"]: "<<cl_[i]<<std::endl;
    std::cout<<"gl["<<i<<"]: "<<gl_[i]<<std::endl;
    std::cout<<"L[x["<<i<<"]]: "<<evalLagrangian(i, X_[i]-xb_)<<std::endl;
  }*/
  
  double fLowest = F_[0];
  xiLowest_ = 0;
  for(i = 1; i < m_; i++)
  {
    if(F_[i] < fLowest)
    {
      xiLowest_ = i;
      fLowest = F_[i];
    }
  }
  
  testInvariants();
}
