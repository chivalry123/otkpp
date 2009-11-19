
#include "UOBYQA.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

bool sort_pred(const std::pair< int, double > &p1, 
               const std::pair< int, double > &p2)
{
  return p1.second > p2.second;
}

std::string UOBYQA::getName() const
{
  return "UOBYQA";
}

bool UOBYQA::hasBuiltInStoppingCriterion() const
{
  return true;
}

bool UOBYQA::isGSLSolver() const
{
  return false;
}

bool UOBYQA::usesGradient() const
{
  return false;
}

bool UOBYQA::usesHessian() const
{
  return false;
}

/*void UOBYQA::computeCenterPoint_()
{
  xc_ = X_[1];
  for(int i = 1; i < n_; i++)
    xc_ += X_[i];
  xc_ /= n_;
}*/

/*void UOBYQA::computeInitModelParams_(const vector< double > &x0)
{
  //double betaj;
  double D;
  double fv;
  double gammaj, gammak;
  int i, j, k;
  double m1, m2, m3, m4;
  double r1, r2;
  vector< double > alpha(n_);
  vector< double > beta(n_);
  double fXBest = f_;
  
  c_ = f_;
  
  X_[0] = x0;
  for(j = 0; j < n_; j++)
  {
    X_[2*j+1] = x0;
    alpha[j] = delta_;
    X_[2*j+1][j] += alpha[j];
    
    if(objFunc_(X_[2*j+1]) < f_)
      beta[j] = 2.0*delta_;
    else
      beta[j] = -2.0*delta_;
    
    X_[2*j+2] = x0;
    X_[2*j+2][j] += beta[j];
    
    /*lDenomHat_[2*j+1] = delta_*delta_ - delta_*betaj;
    lDenomHat_[2*j+2] = betaj*betaj - delta_*betaj;*/
  
    /*m1 = 0.5*delta_*delta_;
    m2 = delta_;
    m3 = 0.5*beta[j]*beta[j];
    m4 = beta[j];
    
    fv = objFunc_(X_[2*j+1]);
    if(fv < fXBest)
    {
      fXBest = fv;
      xiBest_ = 2*j+1;
    }
    r1 = fv - f_;
    fv = objFunc_(X_[2*j+2]);
    if(fv < fXBest)
    {
      fXBest = fv;
      xiBest_ = 2*j+2;
    }
    r2 = fv - f_;
    D = m1*m4 - m2*m3;
    H_(j, j) = (m4*r1 - m2*r2) / D;
    g_(j) = (-m3*r1 + m1*r2) / D;
  }
  
  for(j = 0; j < n_; j++)
  {
    for(k = j+1; k < n_; k++)
    {
      i = 2*n_+1+j+1+k*(k-1)/2-1;
      X_[i] = x0;
      if(objFunc_(X_[2*j+1]) < f_)
        gammaj = delta_;
      else
        gammaj = -delta_;
      if(objFunc_(X_[2*k+1]) < f_)
        gammak = delta_;
      else
        gammak = -delta_;
      
      X_[i][j] += gammaj;
      X_[i][k] += gammak;
      
      //lDenom_[i] = gammaj/gammak;
      
      fv = objFunc_(X_[i]);
      if(fv < fXBest)
      {
        fXBest = fv;
        xiBest_ = i;
      }
      
      H_(j, k) = (objFunc_(X_[i]) - 0.5*gammak*gammak*H_(k, k) - 
                 0.5*gammaj*gammaj*H_(j, j) - gammaj*g_(j) - gammak*g_(k) - c_) / 
                 (gammaj*gammak);
      H_(k, j) = H_(j, k);
      
      computeLagrangeCoeff_last_(x0, gammaj*gammak, i, j, k);
      /*std::cout<<"li(x) 1: "<<cl_[i] + inner_prod(gl_[i], X_[i]-x0) + 0.5*inner_prod(X_[i]-x0, prod(Hl_[i], X_[i]-x0))<<std::endl;
      std::cout<<"li(x) 2: "<<(X_[i][j]-x0[j])*(X_[i][k]-x0[k])/(gammaj*gammak)<<std::endl;*/
    /*}
  }
  
  double denom1, denom2;
  for(j = 0; j < n_; j++)
  {
    denom1 = alpha[j]*alpha[j] - alpha[j]*beta[j];
    denom2 = beta[j]*beta[j] - alpha[j]*beta[j];
    
    computeLagrangeCoeff_hat_(x0, denom1, denom2, j);
  }
  
  for(i = 1; i < 2*n_+1; i++)
  {
    computeLagrangeCoeff_first_(i, x0);
    //std::cout<<"li(x): "<<cl_[i] + inner_prod(gl_[i], X_[i]-x0) + 0.5*inner_prod(X_[i]-x0, prod(Hl_[i], X_[i]-x0))<<std::endl;
  }
  computeLagrangeCoeff_first_(0, x0);
}

void UOBYQA::computeLagrangeCoeff_first_(int i, const vector< double > &x0)
{
  if(i > 0)
  {
    double coeff;
    vector< double > dx;
    
    cl_[i] = cl_hat_[i];
    gl_[i] = gl_hat_[i];
    Hl_[i] = Hl_hat_[i];
    
    /*std::cout<<"TESTING"<<std::endl;
    std::cout<<"cl_[i]: "<<cl_[i]<<std::endl;
    std::cout<<"gl_[i]: "<<gl_[i]<<std::endl;
    std::cout<<"Hl_[i]: "<<Hl_[i]<<std::endl;*/
    
    /*for(int t = 2*n_+1; t < m_; t++)
    {
      dx = X_[t] - x0;
      coeff = 0.5*inner_prod(prod(dx, Hl_hat_[i]), dx) + 
              inner_prod(gl_hat_[i], dx) + 
              cl_hat_[i];
      cl_[i] -= coeff * cl_[t];
      gl_[i] -= coeff * gl_[t];
      Hl_[i] -= coeff * Hl_[t];
    }
  }
  else
  {
    cl_[0] = 1.0;
    gl_[0] = zero_vector< double >(n_);
    Hl_[0] = zero_matrix< double >(n_, n_);
    
    for(int t = 1; t < m_; t++)
    {
      cl_[0] -= cl_[t];
      gl_[0] -= gl_[t];
      Hl_[0] -= Hl_[t];
    }
  }
}

void UOBYQA::computeLagrangeCoeff_last_(const vector< double > &x0, double denom, int i, int j, int k)
{
  Hl_[i] = zero_matrix< double >(n_, n_);
  Hl_[i](j, k) = /*Hl_[i](k, j) =*/ /*2.0 / denom;
  gl_[i] = zero_vector< double >(n_);
  cl_[i] = 0.0;
}

void UOBYQA::computeLagrangeCoeff_hat_(const vector< double > &x0, double denom1, double denom2, int j)
{
  Hl_hat_[2*j+1] = zero_matrix< double >(n_, n_);
  Hl_hat_[2*j+1](j, j) = 2.0 / denom1;
  gl_hat_[2*j+1] = zero_vector< double >(n_);
  gl_hat_[2*j+1][j] = (-X_[2*j+2][j] + x0[j]) / denom1;
  cl_hat_[2*j+1] = 0.0;
  
  Hl_hat_[2*j+2] = zero_matrix< double >(n_, n_);
  Hl_hat_[2*j+2](j, j) = 2.0 / denom2;
  gl_hat_[2*j+2] = zero_vector< double >(n_);
  gl_hat_[2*j+2][j] = (-X_[2*j+1][j] + x0[j]) / denom2;
  cl_hat_[2*j+2] = 0.0;
}*/

double UOBYQA::computeMinEigValEstim_()
{
  const vector< double > &g = model_.getG();
  const matrix< double > &H = model_.getH();
  
  double frobNorm;
  double infNorm;
  int i, j;
  double t;
  double minDiag;
  double sum, maxSum;
  
  frobNorm = norm_frobenius(H);
  infNorm = norm_inf(H);
  
  minDiag = H(0, 0);
  for(i = 1; i < n_; i++)
    minDiag = std::min(H(i, i), minDiag);
  
  maxSum = 0.0;
  for(i = 0; i < n_; i++)
  {
    sum = 0.0;
    sum = -H(i, i);
    for(j = 0; j < n_; j++)
    {
      if(j != i)
        sum += fabs(H(i, j));
    }
    maxSum = std::max(sum, maxSum);
  }
  
  t = norm_2(g) / delta_;
  t += std::min(maxSum, std::min(frobNorm, infNorm));
  
  return std::max(0.0, std::max(-minDiag, t));
}

void UOBYQA::computeModelStep_(int j, vector< double > &d, double &lj)
{
  computeTrsRegStep_(-model_.getGl(j), -model_.getHl(j), rho_, d);
  lj = fabs(model_.evalLagrangian(j, d));
  return;
  
  double a, b, c;
  double colNorm;
  vector< double > D;
  double DD;
  vector< double > dHat;
  vector< double > dTilde;
  vector< double > h;
  int i;
  double estim, maxEstim;
  double phi, maxPhi;
  double cosPhi, sinPhi;
  const vector< double > &g = model_.getGl(j);
  const matrix< double > &H = model_.getHl(j);
  double maxColNorm;
  int maxColIdx;
  double r, r1, r2;
  double theta;
  vector< double > uHat;
  vector< double > uTilde;
  vector< double > w;
  double ww;
  //double lMax;
  
  h = g + prod(H, model_.getLowestX() - model_.getOrigin());
  
  dHat = rho_*h / norm_2(h);
  
  maxColNorm = 0.0;
  maxColIdx = 0;
  for(int i = 0; i < n_; i++)
  {
    colNorm = norm_2(column(H, i));
    if(colNorm > maxColNorm)
    {
      maxColNorm = colNorm;
      maxColIdx = i;
    }
  }
  w = column(H, maxColIdx);
  ww = inner_prod(w, w);
  D = prod(H, w);
  DD = inner_prod(D, D);
  
  /*a = inner_prod(D, prod(H, D)) * inner_prod(w, D) - DD*DD;
  b = inner_prod(D, prod(H, D))*ww - DD*inner_prod(w, D);
  c = DD*ww - inner_prod(w, D)*inner_prod(w, D);
  r1 = (-b + sqrt(b*b-4.0*a*c))/(2.0*a);
  r2 = (-b - sqrt(b*b-4.0*a*c))/(2.0*a);
  if(inner_prod(w+r1*D, prod(H, w+r1*D)) / (inner_prod(w+r1*D, w+r1*D)*inner_prod(w+r1*D, w+r1*D)) > 
     inner_prod(w+r2*D, prod(H, w+r2*D)) / (inner_prod(w+r2*D, w+r2*D)*inner_prod(w+r2*D, w+r2*D)))
    dTilde = w + r1*D;
  else
    dTilde = w + r2*D;
  
  theta = 0.5 * atan(2.0*inner_prod(dHat, prod(H, dTilde)) / 
                    (inner_prod(dHat, prod(H, dHat)) - 
                    inner_prod(dTilde, prod(H, dTilde))));*/
  
  uHat   =  cos(theta)*dTilde + sin(theta)*dHat;
  uTilde = -sin(theta)*dTilde + cos(theta)*dHat;
  
  /*maxEstim = -1.0;
  for(i = 0; i < 8; i++)
  {
    phi = i*M_PI/4.0;
    cosPhi = cos(phi);
    sinPhi = sin(phi);
    estim = fabs(inner_prod(h, uHat)*cosPhi + inner_prod(h, uTilde)*sinPhi + 
                 0.5*inner_prod(uHat, prod(H, uHat))*cosPhi*cosPhi + 
                 0.5*inner_prod(uTilde, prod(H, uTilde))*sinPhi*sinPhi);
    if(estim > maxEstim)
    {
      maxEstim = estim;
      maxPhi = phi;
    }
  }
  
  d = rho_ * (cos(maxPhi)*uHat + sin(maxPhi)*uTilde); // rho??
  lj = maxEstim;*/
  
  double t1 = fabs(inner_prod(h, uHat)) + 0.5*fabs(inner_prod(uHat, prod(H, uHat)));
  double t2 = fabs(inner_prod(h, uTilde)) + 0.5*fabs(inner_prod(uTilde, prod(H, uTilde)));
  double t3 = 1.0/sqrt(2) * (fabs(inner_prod(h, uHat)) + fabs(inner_prod(h, uTilde))) + 
              0.25 * fabs(inner_prod(uHat, prod(H, uHat)) + inner_prod(uTilde, prod(H, uTilde)));
  
  if(t1 >= t2 && t1 >= t3)
    d = rho_ * uHat;
  else if(t2 >= t1 && t2 >= t3)
    d = rho_ * uTilde;
  else
    d = rho_ * (uTilde + uHat) / sqrt(2);
  
  lj = std::max(t1, std::max(t2, t3));
}

double UOBYQA::computeReduction_(const vector< double > &x,
                               const vector< double > &xPlus,
                               const vector< double > &p) const
{
  double ared, pred;
  
  ared = objFunc_(x) - objFunc_(xPlus);
  pred = -inner_prod(model_.getG(), p) - 
          0.5*inner_prod(p, prod(model_.getH(), p));
  
  return ared / pred;
}

/*void UOBYQA::generateInitialPoints_(const vector< double > &x0)
{
  double betaj;
  double fx0;
  double gammaj, gammak;
  int i, j, k;
  
  X_[0] = x0;
  fx0 = objFunc_(x0);
  
  for(j = 0; j < n_; j++)
  {
    X_[2*j+1] = x0;
    X_[2*j+1][j] += delta_;
    
    if(objFunc_(X_[2*j+1]) < fx0)
      betaj = 2.0*delta_;
    else
      betaj = -2.0*delta_;
    
    X_[2*j+2] = x0;
    X_[2*j+2][j] += betaj;
  }
  
  for(j = 0; j < n_; j++)
  {
    for(k = j+1; k < n_; k++)
    {
      //i = 2*n_+1+j+(k-1)*(k-2)/2;
      i = 2*n_+1+j+1+k*(k-1)/2-1;
      X_[i] = x0;
      if(objFunc_(X_[2*j+1]) < fx0)
        gammaj = delta_;
      else
        gammaj = -delta_;
      if(objFunc_(X_[2*k+1]) < fx0)
        gammak = delta_;
      else
        gammak = -delta_;
      
      X_[i][j] += gammaj;
      X_[i][k] += gammak;
    }
  }
}*/

double UOBYQA::computeTau_(const vector< double > &d,
                         const vector< double > &p)
{
  double pd = inner_prod(p, d);
  double d2 = inner_prod(d, d);
  double p2 = inner_prod(p, p);
  
  return (-pd + sqrt(pd*pd - d2*(p2 - delta_*delta_))) / d2;
}

vector< double > &UOBYQA::computeTrsRegStep_(const vector< double > &g,
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

NativeSolver::IterationStatus UOBYQA::iterate_()
{
  double distSq;
  vector< double > dx(n_);
  int i;
  double maxDistSq = 0.0;
  double ratio;
  int t = -1;
  bool trsRegIter = true;
  double dMove = 0.0;
  bool fImproved = false;
  double dNorm = 0.0;
  
  /*std::cout<<"p: "<<p_<<std::endl;
  std::cout<<"fx_k: "<<f_<<std::endl;
  //std::cout<<"fxplus: "<<fXPlus_<<std::endl;
  //std::cout<<"ratio: "<<ratio<<std::endl;
  std::cout<<"delta: "<<delta_<<std::endl;
  std::cout<<"H: "<<H_<<std::endl;
  std::cout<<"g: "<<g_<<std::endl;
  std::cout<<"c: "<<c_<<std::endl;*/
  /*for(int i = 0; i < m_; i++)
    std::cout<<"x["<<i+1<<"]: "<<M_.getX()[i]<<std::endl;*/
  /*testLagrange_();
  if(nIter_ == 10)
    throw std::runtime_error("...");*/
  
  // take a trust region step if necessary
  if(moveIndex_ == -1)
  {
    computeTrsRegStep_(model_.getG(), model_.getH(), delta_, d_);
    
    dNorm = norm_2(d_);
    if(dNorm < 0.5*rho_)
    {
      trsRegIter = false;
      lambda_ = computeMinEigValEstim_();
      if(numMUpdates_ >= 10)
        eps_ = 0.5*rho_*rho_*lambda_;
      else
        eps_ = 0.0;
    }
    else
      eps_ = 0.0;
  }
  else
    t = moveIndex_;
  
  if(trsRegIter == true)
  {
    xPlus_ = model_.getLowestX() + d_;
    fXPlus_ = objFunc_(xPlus_);
    
    if(moveIndex_ == -1)
    {
      // update trust region radius
      ratio = computeReduction_(model_.getLowestX(), xPlus_, d_);
      
      if(ratio >= 0.7)
        delta_ = std::max(delta_, std::max(1.25*dNorm, rho_+dNorm));
      else if(0.1 < ratio && ratio < 0.7)
        delta_ = std::max(0.5*delta_, dNorm);
      else if(ratio <= 0.1)
        delta_ = 0.5*dNorm;
      
      if(delta_ <= 1.5*rho_)
        delta_ = rho_;
      
      vector< double > xHat;
      if(fXPlus_ >= model_.getLowestF())
        xHat = model_.getLowestX();
      else
        xHat = xPlus_;
      
      double testVal;
      double maxTestVal = 0.0;
      for(int j = 0; j < m_; j++)
      {
        dx = xHat - model_.getX()[j];
        
        if(fXPlus_ < model_.getLowestF() || j != model_.getLowestIndex())
        {
          testVal = fabs(model_.evalLagrangian(j, d_)) * 
            std::max(1.0, pow(norm_2(model_.getX()[j]-xHat)/rho_, 3.0));
          if(testVal > maxTestVal)
          {
            maxTestVal = testVal;
            t = j;
          }
        }
      }
      if(maxTestVal <= 1.0 && fXPlus_ >= model_.getLowestF())
        t = -1;
    }
    else
      t = moveIndex_;
    
    if(t != -1)
    {
      dMove = norm_2(model_.getX()[t] - xPlus_);
      fImproved = model_.updatePoint(xPlus_, t);
      updateM_();
      
      if(moveIndex_ >= 0 || fImproved == true || dNorm > 2.0*rho_ || dMove > 2.0*rho_)
      {
        x_ = model_.getLowestX();
        f_ = model_.getLowestF();
        
        moveIndex_ = -1;
        
        if(nIter_ < 1000)
          return NativeSolver::ITERATION_CONTINUE;
        else
          return NativeSolver::ITERATION_SUCCESS;
      }
    }
  }
  
  J_.clear();
  for(int j = 0; j < m_; j++)
  {
    if(j == model_.getLowestIndex())
      continue;
    dx = model_.getLowestX() - model_.getX()[j];
    distSq = inner_prod(dx, dx);
    if(distSq > 4.0*rho_*rho_)
      J_.push_back(std::pair< int, double >(j, distSq));
  }
  std::sort(J_.begin(), J_.end(), sort_pred);
  /*for(int i = 0; i < J_.size(); i++)
    std::cout<<J_[i].first<<" "<<J_[i].second<<std::endl;*/
  
  double lj;
  
  /*while(J_.size() > 0)
  {*/
  maxDistSq = 0.0;
  if(J_.size() == 0)
    moveIndex_ = -1;
  for(int j = 0; j < J_.size(); j++)
  {
    /*dx = model_.getLowestX() - model_.getX()[j];
    if(inner_prod(dx, dx) <= 4.0*rho_*rho_)
      continue;
    distSq = inner_prod(dx, dx);*/
    computeModelStep_(J_[j].first, d_, lj);
    if(M_*pow(norm_2(model_.getX()[J_[j].first] - 
       model_.getLowestX()), 3.0) * 
       fabs(model_.evalLagrangian(J_[j].first, d_)) > eps_)
    {
      //maxDistSq = distSq;
      moveIndex_ = J_[j].first;
      
      model_.updatePoint(model_.getLowestX() + d_, moveIndex_);
      updateM_();
      
      break;
    }
    else if(j == J_.size() - 1)
      moveIndex_ = -1;
  }
  
  if(moveIndex_ >= 0 || (moveIndex_ == -1 && dNorm > rho_))
  {
    x_ = model_.getLowestX();
    f_ = model_.getLowestF();
    
    if(nIter_ < 1000)
      return NativeSolver::ITERATION_CONTINUE;
    else
      return NativeSolver::ITERATION_SUCCESS;
  }
  
  double rhoOld = rho_;
  if(rhoFinal_ < rho_ && rho_ <= 16.0*rhoFinal_)
    rho_ = rhoFinal_;
  else if(16.0*rhoFinal_ < rho_ && rho_ <= 250.0*rhoFinal_)
    rho_ = sqrt(rhoOld*rhoFinal_);
  else if(rho_ > 250.0*rhoFinal_)
    rho_ = 0.1*rhoOld;
  
  delta_ = std::max(0.5*rhoOld, rho_);
  
  /*dx = model_.getOrigin() - model_.getLowestX();
  for(int i = 0; i < m_; i++)
    model_.updatePoint(model_.getX()[i] + dx, i);*/
  model_.setOrigin(model_.getLowestX());
  
  x_ = model_.getLowestX();
  f_ = model_.getLowestF();
  
  //trsRegMinimizer_.computeStep(x_, f_, g_, H_, p, nonZeroStep, xPlus, fXPlus);
  
  /*if(inner_prod(p,p) < 1e-6)
  {
    std::cout<<"H: "<<H_<<std::endl;
    std::cout<<"g: "<<g_<<std::endl;
    std::cout<<"c: "<<c_<<std::endl;
    std::cout<<"xb: "<<xb_<<std::endl;
    std::cout<<"xPlus: "<<xPlus<<std::endl;
    std::cout<<"delta: "<<trsRegMinimizer_.getDelta()<<std::endl;
    throw std::runtime_error("no progress");*/
    /*trsRegMinimizer_.setDelta(0.5*trsRegMinimizer_.getDelta());
    return NativeSolver::ITERATION_CONTINUE;*/
  //}
  
  //testLagrange_();
  //throw std::runtime_error("test completed");
  
  if(nIter_ < 1000)
  {
    if(rho_ > rhoFinal_)
      return NativeSolver::ITERATION_CONTINUE;
    else
      return NativeSolver::ITERATION_SUCCESS;
  }
  else
    return NativeSolver::ITERATION_SUCCESS;
}

NativeSolver::IterationStatus UOBYQA::iterate2_()
{
  double fXPlus;
  double distSq;
  vector< double > dx(n_);
  int i, j;
  double maxDistSq = 0.0;
  bool nonzeroStep = false;
  vector< double > p(n_);
  double ratio;
  int t = 0;
  bool trsRegIter = true;
  vector< double > xPlus(n_);
  double eta1 = 0.15;
  double eta2 = 0.1;

  computeTrsRegStep_(model_.getG(), model_.getH(), delta_, p);
/*trsRegMinimizer_.setDelta(delta_);
  trsRegMinimizer_.computeStep(x_, f_, g_, H_, p, nonzeroStep, xPlus, fXPlus);*/
  xPlus = x_ + p;
  fXPlus = objFunc_(xPlus);
  ratio = computeReduction_(x_, xPlus, p);

  /*std::cout<<"p: "<<p<<std::endl;
  std::cout<<"fx_k: "<<f_<<std::endl;
  std::cout<<"fx_best: "<<fXBest_<<std::endl;
  std::cout<<"fxplus: "<<fXPlus<<std::endl;
  std::cout<<"ratio: "<<ratio<<std::endl;
  std::cout<<"delta: "<<delta_<<std::endl;
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
  if(ratio > eta1)
  {
    for(j = 0; j < m_; j++)
    {
      dx = x_ - model_.getX()[j];
      distSq = inner_prod(dx, dx);
      if(distSq > maxDistSq)
      {
        maxDistSq = distSq;
        t = j;
      }
    }
  
    if(t != -1)
      model_.updatePoint(xPlus, t);
  
    delta_ *= 2.0;
  }
  else
    delta_ *= 0.25;

  double minf = INFINITY;
  int mini = 0;
  for(int i = 0; i < m_; i++)
  {
    bool equals = true;
    for(int j = 0; j < n_; j++)
    {
      if(model_.getX()[i][j] != x_[j])
      {
        equals = false;
        continue;
      }
    }
  
    if(equals == false && model_.getF()[i] < minf)
    {
      minf = model_.getF()[i];
      mini = i;
    }
  }

//ratio = (f_-minf) / (-inner_prod(g_, p) - 0.5*inner_prod(p, prod(H_, p)));

  if(ratio > eta2)
  {
    x_ = model_.getX()[mini];
    f_ = minf;
  
    /*for(int i = 0; i < m_; i++)
    {*/
    /*if(i == mini)
      continue;*/
      /*updateLagrangeCoeff_(M_.getX()[i] /*+ x_ - X_[mini]*//*, i);
    }
    updateModel_();*/
  }

  if(nIter_ < 1000)
    return NativeSolver::ITERATION_CONTINUE;
  else
    return NativeSolver::ITERATION_SUCCESS;
}

void UOBYQA::setup_(const Function &objFunc,
                  const vector< double > &x0,
                  const SolverSetup &solverSetup,
                  const Constraints &C)
{
  NativeSolver::setup_(objFunc, x0, solverSetup);
  //trsRegMinimizer_.setup(objFunc);
  
  delta_ = rho_ = 0.1;
  rhoFinal_ = 1e-8;
  moveIndex_ = -1;
  M_ = 0.0;
  numMUpdates_ = 0;
  
  m_ = (n_+1)*(n_+2)/2;
  
  //X_.resize(m_);
  /*lDenom_.resize(n_);
  lDenomHat_.resize(n_);*/
  /*for(int i = 0; i < m_; i++)
    X_[i].resize(n_);
  F_.resize(m_);
  g_.resize(n_);
  H_.resize(n_, n_);*/
  
  d_.resize(n_);
  
  /*cl_.resize(m_);
  gl_.resize(m_);
  for(int i = 0; i < m_; i++)
    gl_[i].resize(n_);
  Hl_.resize(m_);
  for(int i = 0; i < m_; i++)
    Hl_[i].resize(n_, n_);
  
  cl_hat_.resize(2*n_ + 1);
  gl_hat_.resize(2*n_ + 1);
  for(int i = 0; i < 2*n_+1; i++)
    gl_hat_[i].resize(n_);
  Hl_hat_.resize(2*n_ + 1);
  for(int i = 0; i < 2*n_+1; i++)
    Hl_hat_[i].resize(n_, n_);*/
  
  //generateInitialPoints_(x0);
  
  x_ = x0;
  f_ = objFunc_(x0);
  xPlus_ = x0;
  fXPlus_ = f_;
  
  //xiBest_ = 0;
  
  model_ = QuadInterp(objFunc, x0, delta_);
  //computeInitModelParams_(x0);
  /*for(int i = 0; i < m_; i++)
    F_[i] = objFunc_(X_[i]);*/
  
  /*for(int i = 0; i < m_; i++)
    std::cout<<X_[i][0]<<" "<<X_[i][1]<<std::endl;
  matrix< double > H(n_, n_);
  objFunc_.H(x0, H);
  std::cout<<H<<std::endl;
  std::cout<<H_<<std::endl;*/
  
  /*testLagrange_();
  throw std::runtime_error("test completed");*/
}

/*void UOBYQA::updateLagrangeCoeff_(const vector< double > &xHat, int j)
{
  double c1, c2;
  vector< double > dx = xHat - x_;
  int i;
  
  c2 = 0.5*inner_prod(dx, prod(Hl_[j], dx)) + inner_prod(gl_[j], dx) + cl_[j];
  
  cl_[j] /= c2;
  gl_[j] /= c2;
  Hl_[j] /= c2;
  
  for(i = 0; i < m_; i++)
  {
    if(i == j)
      continue;
    
    c1 = 0.5*inner_prod(dx, prod(Hl_[i], dx)) + inner_prod(gl_[i], dx) + cl_[i];
    
    cl_[i] -= c1*cl_[j];
    gl_[i] -= c1*gl_[j];
    Hl_[i] -= c1*Hl_[j];
  }
  
  //throw std::runtime_error("test finished");
}*/

/*void UOBYQA::updateModel_()
{
  //std::cout<<"H before: "<<H_<<std::endl;
  
  c_ = 0.0;
  g_ = zero_vector< double >(n_);
  H_ = zero_matrix< double >(n_, n_);
  for(int i = 0; i < m_; i++)
  {
    c_ += F_[i] * cl_[i];
    g_ += F_[i] * gl_[i];
    H_ += F_[i] * Hl_[i];
  }
  
  for(int i = 0; i < n_; i++)
    for(int j = 0; j < i; j++)
      H_(i, j) = H_(j, i);
  
  //std::cout<<"H after: "<<H_<<std::endl;
}*/

/*void UOBYQA::testLagrange_()
{
  double Lx;
  double ljx;
  vector< double > x;
  
  for(int i = 0; i < m_; i++)
  {
    Lx = 0;
    x = X_[i];
    std::cout<<"X["<<i+1<<"]:"<<std::endl;
    std::cout<<"f(x)="<<objFunc_(x)<<std::endl;
    std::cout<<"Q(x)="<<c_ + inner_prod(g_, x-X_[xiBest_]) + 0.5 * inner_prod(x-X_[xiBest_], prod(H_, x-X_[xiBest_]))<<std::endl;
    for(int j = 0; j < m_; j++)
    {
      /*std::cout<<"clj: "<<cl_[j]<<std::endl;
      std::cout<<"glj: "<<gl_[j]<<std::endl;
      std::cout<<"Hlj: "<<Hl_[j]<<std::endl;*/
      /*ljx = cl_[j] + inner_prod(gl_[j], x-X_[xiBest_]) + 0.5*inner_prod(x-X_[xiBest_], prod(Hl_[j], x-X_[xiBest_]));
      std::cout<<"lj[x]: "<<ljx<<std::endl;
      Lx += objFunc_(X_[j]) * ljx;
    }
    std::cout<<"L(x)="<<Lx<<std::endl;
  }
}*/

void UOBYQA::updateM_()
{
  double num = fabs(model_.eval(d_) - objFunc_(model_.getLowestX() + d_));
  double denom = 0.0;
  
  for(int i = 0; i < m_; i++)
    denom += fabs(model_.evalLagrangian(i, d_)) * 
      pow(norm_2(model_.getLowestX() + d_ - model_.getX()[i]), 3.0);
  
  M_ = std::max(M_, num/denom);
  
  numMUpdates_++;
}
