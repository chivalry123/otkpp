
#include "NativeLBFGSB.h"

/*#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <list>

struct StepComparator
{
  bool operator()(const std::pair< int, double > &p1,
                  const std::pair< int, double > &p2)
  {
    return p2.first < p1.first;
  }
};

void NativeLBFGSB::computeGenCP_(vector< double > &x_cp)
{
  int b;
  vector< double > c;
  double delta_t;
  double delta_t_min;
  vector< double > t(n_);
  vector< double > d(n_);
  std::vector< std::pair< int, double > > F;
  std::pair< int, double > F_min;
  double fa, faa;
  vector< double > p;
  double t_min;
  double t_old;
  vector< double > z(n_);
  
  // ??
  matrix< double > M;
  double phi;
  matrix< double > W;
  
  for(int i = 0; i < n_; i++)
  {
    if(g_[i] < 0.0)
      t[i] = (x_[i] - u_[i]) / g_[i];
    else if(g_[i] > 0.0)
      t[i] = (x_[i] - l_[i]) / g_[i];
    else
      t[i] = NAN;
    
    if(t[i] > 0)
      F.push_back(std::pair< int, double >(i, t[i]));
  }
  
  p = prod(trans(W), d);
  c = zero_vector< double >(2*m_);
  fa = -inner_prod(d, d);
  faa = -phi*fa - inner_prod(p, prod(M, p));
  delta_t_min = -fa / faa;
  t_old = 0.0;
  sort(F.begin(), F.end(), StepComparator());
  F_min = F.back();
  F.pop_back();
  b = F_min.first;
  delta_t = t_min = F_min.second;
  
  while(delta_t_min >= delta_t)
  {
    if(d[b] > 0.0)
      x_cp[b] = u_[b];
    else
      x_cp[b] = l_[b];
    z[b] = x_cp[b] - x_[b];
    c += delta_t*p;
    matrix_row< matrix< double > > wb(W, b);
    fa = fa + delta_t*faa + 
         g_[b]*g_[b] + phi*g_[b]*z[b] - 
         g_[b]*inner_prod(wb, prod(M, c));
    faa = faa - phi*g_[b]*g_[b] - 
          2.0*g_[b]*inner_prod(wb, prod(M, p)) - 
          g_[b]*g_[b]*inner_prod(wb, prod(M, wb));
    p += g_[b]*wb;
    d[b] = 0.0;
    delta_t_min = -fa / faa;
    t_old = t_min;
    F_min = F.back();
    F.pop_back();
    b = F_min.first;
    t_min = F_min.second;
    delta_t = t_min - t_old;
  }
  
  delta_t_min = std::max(delta_t_min, 0.0);
  t_old += delta_t_min;
  for(int i = 0; i < n_; i++)
  {
    if(t[i] >= t_min)
      x_cp[i] = x_[i] + t_old*d[i];
  }
  c += delta_t_min*p;
}

NativeSolver::IterationStatus NativeLBFGSB::iterate_()
{
  
}

void NativeLBFGSB::setup_(const Function &objFunc,
                          const vector< double > &x0,
                          const SolverSetup &solverSetup,
                          const Constraints &C)
{
  const int n = objFunc.getN();
  
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  
  l_.resize(n);
  u_.resize(n);
  
  // TODO: a temporary hack
  l_[0] = -1;
  l_[1] = 1;
  u_[0] = -1;
  u_[1] = 1;
  //
}*/
