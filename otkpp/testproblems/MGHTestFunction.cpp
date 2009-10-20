
#include "MGHTestFunction.h"

double PowellBadlyScaled_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  
  double x1 = x[0];
  double x2 = x[1];
  
  double fx1 = 10000.0 * x1 * x2 - 1.0;
  double fx2 = exp(-x1) + exp(-x2) - 1.0001;
  
  return fx1 * fx1 + fx2 * fx2;
}

double BrownBadlyScaled_evaluator::eval_(const double *x) const
{
  double x1 = x[0];
  double x2 = x[1];
  
  double f1 = x1 - 1e6;
  double f2 = x2 - 2.0 * 1e-6;
  double f3 = x1 * x2 - 2.0;
  
  return f1 * f1 + f2 * f2 + f3 * f3;
}

double Beale_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  
  double x1 = x[0];
  double x2 = x[1];
  
  double f1 = (1.5   - x1 * (1.0 - x2));
  double f2 = (2.25  - x1 * (1.0 - x2*x2));
  double f3 = (2.625 - x1 * (1.0 - x2*x2*x2));
  
  fx += f1 * f1;
  fx += f2 * f2;
  fx += f3 * f3;
  
  return fx;
}

double HelicalValley_evaluator::eval_(const double *x) const
{
  double phi;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  
  double second_term;
  
  if(x1 > 0.0)
    phi = 1.0 / (2.0 * M_PI) * atan(x2 / x1);
  else
    phi = 1.0 / (2.0 * M_PI) * atan(x2 / x1) + 0.5;
  
  second_term = 10.0 * (sqrt(x1*x1 + x2*x2) - 1.0);
  
  return 100.0 * (x3 - 10.0*phi) * (x3 - 10.0*phi) + 
    second_term * second_term + 
    x3 * x3;
}

static const double GAUSSIAN_Y[] =
{
  0.0009, 0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 
  0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044, 0.0009
};

double Gaussian_evaluator::eval_(const double *x) const
{
  double fix;
  double ti;
  
  double fx = 0.0;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  
  for(int i = 1; i <= 15; i++)
  {
    ti = (8.0 - i) / 2.0;
    fix = x1 * exp(-x2 * (ti-x3) * (ti-x3) / 2.0) - GAUSSIAN_Y[i - 1];
    fx += fix * fix;
  }
  
  return fx;
}

double Gulf_evaluator::eval_(const double *x) const
{
  double fix;
  double ti;
  double yi;
  
  double fx = 0.0;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  
  for(int i = 1; i <= m_; i++)
  {
    ti = i / 100.0;
    yi = 25.0 + pow(-50.0 * log(ti), 2.0 / 3.0);
    fix = exp(-pow(fabs(yi - x2), x3) / x1) - ti;
    fx += fix * fix;
  }
  
  return fx;
}

double Box_evaluator::eval_(const double *x) const
{
  double fix;
  double ti;
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  
  double fx = 0.0;
  
  for(int i = 1; i <= m_; i++)
  {
    ti = 0.1 * i;
    fix = exp(-ti * x1) - exp(-ti * x2) - x3 * (exp(-ti) - exp(-10.0 * ti));
    fx += fix * fix;
  }
  
  return fx;
}

double Wood_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  double x4 = x[3];
  
  fx += 100.0 * (x2 - x1 * x1) * (x2 - x1 * x1);
  fx += (1.0 - x1) * (1.0 - x1);
  fx += 90.0 * (x4 - x3 * x3) * (x4 - x3 * x3);
  fx += (1.0 - x3) * (1.0 - x3);
  fx += 10.0 * (x2 + x4 - 2.0) * (x2 + x4 - 2.0);
  fx += 0.1 * (x2 - x4) * (x2 - x4);
  
  return fx;
}

double BrownDennis_evaluator::eval_(const double *x) const
{
  double ti;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  double x4 = x[3];
  
  double fix;
  double term1, term2;
  
  double fx = 0.0;
  
  for(int i = 1; i <= m_; i++)
  {
    ti = i / 5.0;
    
    term1 = x1 + ti * x2 - exp(ti);
    term2 = x3 + x4 * sin(ti) - cos(ti);
    
    fix = term1 * term1 + term2 * term2;
    fx += fix * fix;
  }
  
  return fx;
}

double BiggsEXP6_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  double fix;
  double ti;
  double yi;
  
  double x1 = x[0];
  double x2 = x[1];
  double x3 = x[2];
  double x4 = x[3];
  double x5 = x[4];
  double x6 = x[5];
  
  for(int i = 1; i <= m_; i++)
  {
    ti = 0.1 * i;
    yi = exp(-ti) - 5.0*exp(-10.0*ti) + 3.0*exp(-4.0*ti);
    fix = x3*exp(-ti*x1) - x4*exp(-ti*x2) + x6*exp(-ti*x5) - yi;
    fx += fix * fix;
  }
  
  return fx;
}

double Watson_evaluator::eval_(const double *x) const
{
  double fix;
  int i, j;
  double s;
  double ti;
  double tip;
  
  double fx = 0.0;
  
  for(i = 1; i <= 29; i++)
  {
    fix = 0.0;
    
    ti = ((double)i) / 29.0;
    s = 0.0;
    tip = 1.0;
    for(j = 2; j <= n_; j++)
    {
      s += (j - 1) * x[j - 1] * tip;
      tip *= ti;
    }
    fix += s;
    
    s = 0.0;
    tip = 1.0;
    for(j = 1; j <= n_; j++)
    {
      s += x[j - 1] * tip;
      tip *= ti;
    }
    fix -= s*s + 1.0;
    
    fx += fix * fix;
  }
  
  double x1 = x[0];
  double x2 = x[1];
  
  fx += x1 * x1;
  fx += (x2 - x1 * x1 - 1.0) * (x2 - x1 * x1 - 1.0);
  
  return fx;
}

double ExtendedRosenbrock_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  
  double f1, f2;
  double x1, x2;
  
  for(int i = 0; i < n_; i += 2)
  {
    x1 = x[i];
    x2 = x[i + 1];
    
    f1 = 10.0 * (x2 - x1*x1);
    f2 = 1.0 - x1;
    
    fx += f1*f1 + f2*f2;
  }
  
  return fx;
}

double ExtendedPowellSingular_evaluator::eval_(const double *x) const
{
  double fix;
  double x1, x2, x3, x4;
  
  double fx = 0.0;
  
  for(int i = 0; i < n_; i += 4)
  {
    x1 = x[i];
    x2 = x[i + 1];
    x3 = x[i + 2];
    x4 = x[i + 3];
    
    fix = x1 + 10.0 * x2;
    fx += fix * fix;
    
    fix = x3 - x4;
    fx += 5.0 * fix * fix;
    
    fix = (x2 - 2.0 * x3) * (x2 - 2.0 * x3);
    fx += fix * fix;
    
    fix = (x1 - x4) * (x1 - x4);
    fx += 10.0 * fix * fix;
  }
  
  return fx;
}

double PenaltyFunctionI_evaluator::eval_(const double *x) const
{
  double fx = 0.0;
  
  double fix;
  int i;
  double sum = 0.0;
  
  for(int i = 1; i <= n_; i++)
  {
    fix = x[i - 1] - 1.0;
    fx += 1e-5 * fix * fix;
  }
  
  for(i = 1; i <= n_; i++)
    sum += x[i - 1] * x[i - 1];
  
  fx += (sum - 0.25) * (sum - 0.25);
  
  return fx;
}

double PenaltyFunctionII_evaluator::eval_(const double *x) const
{
  double fix;
  double sum = 0.0;
  double yi;
  
  double fx = 0.0;
  
  fx += (x[0] - 0.2) * (x[0] - 0.2);
  
  for(int i = 2; i < 2*n_; i++)
  {
    yi = exp(i / 10.0) + exp((i - 1) / 10.0);
    
    if(i <= n_)
      fix = exp(x[i - 1] / 10.0) + exp(x[i - 2] / 10.0) - yi;
    else
      fix = exp(x[i - n_] / 10.0) - exp(-1.0 / 10.0);
    
    fx += 1e-5 * fix * fix;
  }
  
  for(int i = 1; i <= n_; i++)
    sum += (n_ - i + 1) * x[i - 1] * x[i - 1];
  
  fx += (sum - 1.0) * (sum - 1.0);
  
  return fx;
}

double VariablyDimensioned_evaluator::eval_(const double *x) const
{
  double sum = 0.0;
  double fx = 0.0;
  
  for(int i = 1; i <= n_; i++)
  {
    fx += (x[i - 1] - 1.0) * (x[i - 1] - 1.0);
    sum += i * (x[i - 1] - 1.0);
  }
  
  fx += sum * sum;
  fx += sum * sum * sum * sum;
  
  return fx;
}

double Trigonometric_evaluator::eval_(const double *x) const
{
  double fix;
  int i, j;
  
  double fx = 0.0;
  
  for(i = 1; i <= n_; i++)
  {
    fix = 0.0;
    
    fix += n_;
    for(j = 1; j <= n_; j++)
      fix -= cos(x[j - 1]);
    fix += i * (1.0 - cos(x[i - 1])) - sin(x[i - 1]);
    
    fx += fix * fix;
  }
  
  return fx;
}

double ChebyQuad_evaluator::eval_(const double *x) const
{
  double fix;
  int i, j, k;
  double sum;
  double Ti, Tim1, prevTi;
  
  double fx = 0.0;
  
  for(i = 1; i <= m_; i++)
  {
    sum = 0.0;
    for(j = 1; j <= n_; j++)
    {
      Tim1 = 1.0;
      Ti = x[j - 1];
      for(k = 2; k <= i; k++)
      {
        prevTi = Ti;
        Ti = 2.0 * x[j - 1] * Ti - Tim1;
        Tim1 = prevTi;
      }
      
      sum += Ti;
    }
    sum /= n_;
    
    fix = sum;
    
    if(i % 2 == 0)
      fix += 1.0 / (i * i - 1.0);
    
    fx += fix * fix;
  }
  
  return fx;
}

bool ExtendedRosenbrock::checkDimensions(int n) const
{
  return n > 0 && n % 2 == 0;
}

bool Gulf::checkDimensions(int m) const
{
  return m >= 3 && m <= 100;
}

bool Box::checkDimensions(int m) const
{
  return m >= 3;
}

bool BrownDennis::checkDimensions(int m) const
{
  return m >= 4;
}

bool BiggsEXP6::checkDimensions(int m) const
{
  return m >= 6;
}

bool Watson::checkDimensions(int n) const
{
  return n >= 2 && n <= 31;
}

bool ExtendedPowellSingular::checkDimensions(int n) const
{
  return n > 0 && n % 4 == 0;
}

bool PenaltyFunctionI::checkDimensions(int n) const
{
  return n > 0;
}

bool PenaltyFunctionII::checkDimensions(int n) const
{
  return n > 0;
}

bool VariablyDimensioned::checkDimensions(int n) const
{
  return n > 0;
}

bool Trigonometric::checkDimensions(int n) const
{
  return n > 0;
}

bool ChebyQuad::checkDimensions(int n, int m) const
{
  return m >= n && n > 0;
}

/*MGHTestFunction::MGHTestFunction(FunctionName fName,
                                 FuncEvalType fEvalType,
                                 DerivEvalType gEvalType)
{
  if(fEvalType == COMPILED)
  {
    switch(fName)
    {
    case BEALE:
      evaluator_ = new CompiledFunctionEvaluator(&beale_f);
      break;
    case HELICALVALLEY:
      evaluator_ = new CompiledFunctionEvaluator(&helicalvalley_f);
      break;
    }
  }
}*/
