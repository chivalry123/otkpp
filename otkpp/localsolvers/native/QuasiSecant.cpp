
#include "cholesky.hpp"
#include "QuasiSecant.h"

#include <boost/numeric/ublas/matrix_proxy.hpp>

void QuasiSecant::addMatrixCol_(const vector< double > &c, matrix< double > &M)
{
  int i, j;
  matrix< double > M_;
  
  M_.resize(M.size1(), M.size2() + 1);
  for(i = 0; i < M.size1(); i++)
    for(j = 0; j < M.size2(); j++)
      M_(i, j) = M(i, j);
  
  for(i = 0; i < M.size1(); i++)
    M_(i, M.size2()) = c[i];
}

void QuasiSecant::computeDescentDirection_()
{
  
}

NativeSolver::IterationStatus QuasiSecant::iterate_()
{
  
}

void QuasiSecant::polyMin_(const matrix< double > &P, vector< double > &pMin)
{
  const int n = P.size1();
  const int m = P.size2();
  
  const double Z1 = 1e-8;
  const double Z2 = 1e-10;
  const double Z3 = 1e-10;
  
  vector< double > es, ev;
  int j, J;
  double maxVal, minVal;
  int nIter;
  std::list< int > pos;
  matrix< double > M;
  matrix< double > Q;
  std::list< int > S;
  int step;
  double testVal;
  vector< double > w;
  vector< double > X;
  vector< double > u, v;
  double theta;
  
  // Step 0.
  minVal = inner_prod(column(P, 0), column(P, 0));
  for(j = 1; j < m; j++)
  {
    testVal = inner_prod(column(P, j), column(P, j));
    if(testVal < minVal)
    {
      minVal = testVal;
      J = j;
    }
  }
  S.push_back(J);
  w.resize(w.size() + 1);
  w[w.size() - 1] = 1.0;
  Q.resize(n, 1);
  column(Q, 0) = column(P, J);
  es.resize(1);
  es[0] = 1.0;
  
  step = 1;
  nIter = 0;
  while(nIter < m)
  {
    if(step == 1)
    {
      // Step 1.
      // (a)
      X = prod(Q, w);
      // (b)
      minVal = INFINITY;
      J = 1;
      for(j = 0; j < m; j++)
      {
        testVal = inner_prod(X, column(P, j));
        if(testVal < minVal)
        {
          minVal = testVal;
          J = j;
        }
      }
      // (c)
      maxVal = 0.0;
      for(j = 0; j < m; j++)
      {
        testVal = inner_prod(column(P, j), column(P, j));
        if(testVal > maxVal)
          maxVal = testVal;
      }
      if(inner_prod(X, column(P, J)) > 
         inner_prod(X, X)-Z1*std::max(inner_prod(column(P, J), 
                    column(P, J)), maxVal))
        return;
      // (d)
      for(std::list< int >::const_iterator Sj = S.begin(); Sj != S.end(); Sj++)
        if(J == (*Sj))
          return;
      // (e)
      S.push_back(J);
      w.resize(w.size()+1);
      w[w.size() - 1] = 0.0;
      addMatrixCol_(column(P, J), Q);
      es.resize(es.size() + 1);
      es[es.size() - 1] = 1.0;
    }
    
    // Step 2.
    // (a)
    M = outer_prod(es, es) + prod(trans(Q), Q);
    u = es;
    cholesky_solve(M, u);
    v = u / inner_prod(es, u);
    // (b)
    bool test = true;
    for(j = 0; j < v.size(); j++)
    {
      if(v[j] <= Z2)
      {
        test = false;
        break;
      }
    }
    if(test == true)
    {
      w = v;
      step = 1;
      continue;
    }
    
    // Step 3.
    // (a)
    pos.clear();
    for(j = 0; j < S.size(); j++)
    {
      if(w[j] - v[j] > Z3)
        pos.push_back(j);
    }
    // (b)
    minVal = INFINITY;
    for(std::list< int >::const_iterator pj = pos.begin(); pj != pos.end(); pj++)
    {
      testVal = w[*pj] / (w[*pj]-v[*pj]);
      if(testVal < minVal)
        minVal = testVal;
    }
    theta = std::min(1.0, minVal);
    // (c)
    w = theta*w + (1.0-theta)*v;
    // (d)
    for(j = 0; j < w.size(); j++)
      if(w[j] <= Z2)
        w[j] = 0.0;
    // (e)
    std::list< int >::iterator Sj = S.begin();
    for(j = 0; j < w.size(); j++)
    {
      if(w[j] == 0)
      {
        w.erase_element(j);
        S.erase(Sj);
        removeMatrixCol_(j, Q);
        es.resize(es.size() - 1);
        break;
      }
      Sj++;
    }
    step = 2;
    nIter++;
  }
}

void QuasiSecant::removeMatrixCol_(int j, matrix< double > &M)
{
  int i, k;
  matrix< double > M_;
  
  M_.resize(M.size1(), M.size2() - 1);
  for(i = 0; i < M.size1(); i++)
  {
    for(k = 0; k <= j; k++)
      M_(i, k) = M(i, k);
    
    for(k = j; k < M.size2(); k++)
      M_(i, k) = M(i, k+1);
  }
}
