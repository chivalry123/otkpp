
#include "NativeNEWUOA.h"

/*#include <boost/numeric/ublas/lu.hpp>
#include <typeinfo>

bool NativeNEWUOASetup::isCompatibleWith(const Solver &s) const
{
  return (typeid(s) == typeid(const NativeNEWUOASetup &));
}

void NativeNEWUOA::computeDirection_(int n, vector< double > &d)
{
  d = zero_vector< double >(n);
  vector< double > s;
  vector< double > g0 = prod(A_, x_) + b_;
  s = -g0;
  double alpha, alphaHat, beta;
  vector< double > dMinus = zero_vector< double >(n);
  vector< double > dPlus = zero_vector< double >(n);
  vector< double > g1, g2;
  vector< double > xPlus;
  double Sxs;
  double Sss;
  double Sxx;
  
  do
  {
    xPlus = x_ + dPlus;
    Sxs = 0.0;
    Sss = 0.0;
    Sxx = 0.0;
    for(int i = 0; i < n; i++)
    {
      Sxs += x_[i] * s[i];
      Sss += s[i] * s[i];
      Sxx += x_[i] * x_[i];
    }
    alphaHat = (-Sxs + sqrt(Sxs*Sxs - Sss*(Sxx - delta_*delta_))) / Sss;
    
    g1 = (prod(A_, x_+dMinus) + b_);
    g2 = (prod(A_, x_+d) + b_);
    
    if(-inner_prod(g2, g2) + alphaHat * inner_prod(s, prod(A_, s)) <= 0.0)
      alpha = alphaHat;
    else
      alpha = -inner_prod(g2, g2) / inner_prod(s, prod(A_, s));
    
    dPlus = d + alpha * s;
    
    beta = inner_prod(g2, g2) / inner_prod(g1, g1);
    s = -(prod(A_, x_+d) + b_) + beta*s;
    
    dMinus = d;
    d = dPlus;
  }
  while(norm_2(g2) > 1e-2 * norm_2(g0) && 
        Q_(x_ + dMinus) - Q_(x_ + d) > 1e-2 * (Q_(x_) - Q_(x_ + d)));
}

void NativeNEWUOA::computeInitialModel_(const Function &f, matrix< double > &A, vector< double > &b, double &c)
{
  const int n = f.getN();
  int i;
  
  matrix< double > M = zero_matrix< double >(2*n+1, 2*n+1);
  for(i = 0; i < n; i++)
  {
    M(i, i)   = 1.0;
    M(i, i+n) = 1.0;
    M(i, 2*n) = 1.0;
    
    M(n+i, i)   = 1.0;
    M(n+i, i+n) = 1.0;
    M(n+i, 2*n) = 1.0;
  }
  vector< double > fv(2*n+1);
  for(i = 0; i < n; i++)
    fv[i] = fv[n+i] = f(X_[i]);
  
  permutation_matrix< double > P(2*n+1);
  lu_factorize(M, P);
  vector< double > x = fv;
  lu_substitute(M, P, x);
  
  A = zero_matrix< double >(n, n);
  b = zero_vector< double >(n);
  for(i = 0; i < n; i++)
  {
    A(i, i) = x(i);
    b(i)    = x(n+i);
  }
  c = x(2*n);
}

double NativeNEWUOA::Q_(const vector< double > &x)
{
  return inner_prod(prod(x, A_), x) + inner_prod(b_, x) + c_;
}

void NativeNEWUOA::setup_(const Function &objFunc,
                          const vector< double > &x0,
                          const SolverSetup &solverSetup)
{
  const int n = objFunc.getN();
  
  if(typeid(solverSetup) == typeid(const DefaultSolverSetup &))
  {
    solverSetup_.m = 2*n+1;
    solverSetup_.rho0 = 100.0;
    solverSetup_.rhoFinal = 1e-15;
  }
  else
    solverSetup_ = dynamic_cast< const NativeNEWUOASetup & >(solverSetup);
  
  if(solverSetup_.m < n + 2 || solverSetup_.m > (n+1)*(n+2)/2)
    throw std::invalid_argument("must be n+2 <= m <= (n+1)(n+2)/2");
  
  // TODO: remove this restriction
  if(solverSetup_.m != 2*n+1)
     throw std::invalid_argument("must be m == 2n+1");
  //
  
  X_.resize(n);
  for(int i = 0; i < n; i++)
    X_[i].resize(n);
  
  // compute initial interpolation points
  X_[0] = x0;
  for(int i = 1; i < n; i++)
  {
    X_[i] = x0;
    X_[i][i] += solverSetup_.rho0;
    X_[i+n+1] = x0;
    X_[i+n+1][i] -= solverSetup_.rho0;
  }
  
  computeInitialModel_(objFunc, A_, b_, c_);
  rho_ = delta_ = solverSetup_.rho0;
}

void NativeNEWUOA::TRSolve_()
{
  
}*/
