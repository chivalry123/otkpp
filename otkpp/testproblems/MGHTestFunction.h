
/*
 * This file contains test functions described in 
 * More, Garbow and Hillstrom: Testing Unconstrained 
 * Optimization Software (1981)
 */

#ifndef MGHTESTFUNCTION_H

#include <otkpp/lib/Cloneable.h>
#include <otkpp/function/Function.h>
#include <otkpp/function/FunctionEvaluator.h>

template < typename T, int N > class TFEvaluator_fixed_n : 
  public Cloneable< T, FunctionEvaluator >
{
 public:
  int getN() const { return N; }
};

template< typename T > class TFEvaluator_arbitrary_n : 
  public Cloneable< T, FunctionEvaluator >
{
 public:
  TFEvaluator_arbitrary_n(int n) : n_(n) { }
  
  int getN() const { return n_; }
 protected:
  int n_;
};

template< typename T, int N > class TFEvaluator_arbitrary_m : 
  public Cloneable< T, FunctionEvaluator >
{
 public:
  TFEvaluator_arbitrary_m(int m) : m_(m) { }
  
  int getN() const { return N; }
 protected:
  int m_;
};

template< typename T > class TFEvaluator_arbitrary_nm : 
  public Cloneable< T, FunctionEvaluator >
{
 public:
  TFEvaluator_arbitrary_nm(int n, int m) : n_(n), m_(m) { }
  
  int getN() const { return n_; }
 protected:
  int n_, m_;
};

#define FUNCTION_EVALUATOR(class_name, n) \
  class class_name : public TFEvaluator_fixed_n< class_name, n > \
  { \
   private: \
    double eval_(const double *x) const; \
  }

#define N_FUNCTION_EVALUATOR(class_name) \
  class class_name : public TFEvaluator_arbitrary_n< class_name > \
  { \
   public: \
    class_name(int n) : \
    TFEvaluator_arbitrary_n< class_name >(n) { } \
   private: \
    double eval_(const double *x) const; \
  }

#define M_FUNCTION_EVALUATOR(class_name, n) \
  class class_name : public TFEvaluator_arbitrary_m< class_name, n > \
  { \
   public: \
    class_name(int m) : \
    TFEvaluator_arbitrary_m< class_name, n >(m) { } \
   private: \
    double eval_(const double *x) const; \
  }

#define NM_FUNCTION_EVALUATOR(class_name) \
  class class_name : public TFEvaluator_arbitrary_nm< class_name > \
  { \
   public: \
    class_name(int n, int m) : \
      TFEvaluator_arbitrary_nm< class_name >(n, m) { } \
   private: \
    double eval_(const double *x) const; \
  }

FUNCTION_EVALUATOR    (PowellBadlyScaled_evaluator, 2);
FUNCTION_EVALUATOR    (BrownBadlyScaled_evaluator, 2);
FUNCTION_EVALUATOR    (Beale_evaluator, 2);
FUNCTION_EVALUATOR    (HelicalValley_evaluator, 3);
FUNCTION_EVALUATOR    (Gaussian_evaluator, 3);
N_FUNCTION_EVALUATOR  (ExtendedRosenbrock_evaluator);
M_FUNCTION_EVALUATOR  (Gulf_evaluator, 3);
M_FUNCTION_EVALUATOR  (Box_evaluator, 3);
FUNCTION_EVALUATOR    (Wood_evaluator, 4);
M_FUNCTION_EVALUATOR  (BrownDennis_evaluator, 4);
M_FUNCTION_EVALUATOR  (BiggsEXP6_evaluator, 6);
N_FUNCTION_EVALUATOR  (Watson_evaluator);
N_FUNCTION_EVALUATOR  (ExtendedPowellSingular_evaluator);
N_FUNCTION_EVALUATOR  (PenaltyFunctionI_evaluator);
N_FUNCTION_EVALUATOR  (PenaltyFunctionII_evaluator);
N_FUNCTION_EVALUATOR  (VariablyDimensioned_evaluator);
N_FUNCTION_EVALUATOR  (Trigonometric_evaluator);
NM_FUNCTION_EVALUATOR (ChebyQuad_evaluator);

class MGHTestFunction_arbitrary_n : public Function
{
 public:
  MGHTestFunction_arbitrary_n(const FunctionEvaluator &fEval,
                              DerivEvalType gEvalType) : 
    Function(fEval, gEvalType) { }
  virtual ~MGHTestFunction_arbitrary_n() { }
  virtual bool checkDimensions(int n) const = 0;
};

class MGHTestFunction_arbitrary_m: public Function
{
 public:
  MGHTestFunction_arbitrary_m(const FunctionEvaluator &fEval,
                              DerivEvalType gEvalType) : 
    Function(fEval, gEvalType) { }
  virtual ~MGHTestFunction_arbitrary_m() { }
  virtual bool checkDimensions(int m) const = 0;
};

class MGHTestFunction_arbitrary_nm : public Function
{
 public:
  MGHTestFunction_arbitrary_nm(const FunctionEvaluator &fEval,
                               DerivEvalType gEvalType) : 
    Function(fEval, gEvalType) { }
  virtual ~MGHTestFunction_arbitrary_nm() { }
  virtual bool checkDimensions(int n, int m) const = 0;
};

#define FUNCTION(class_name, eval_name) \
  class class_name : public Function \
  { \
   public: \
    class_name(Function::DerivEvalType dEvalType) : \
      Function(eval_name(), dEvalType) { } \
  }

#define N_FUNCTION(class_name, eval_name) \
  class class_name : public MGHTestFunction_arbitrary_n \
  { \
   public: \
    class_name(int n, Function::DerivEvalType dEvalType) : \
      MGHTestFunction_arbitrary_n(eval_name(n), dEvalType) { } \
    bool checkDimensions(int n) const; \
  }

#define M_FUNCTION(class_name, eval_name) \
  class class_name : public MGHTestFunction_arbitrary_m \
  { \
   public: \
    class_name(int m, Function::DerivEvalType dEvalType) : \
      MGHTestFunction_arbitrary_m(eval_name(m), dEvalType) { } \
    bool checkDimensions(int m) const; \
  }

#define NM_FUNCTION(class_name, eval_name) \
  class class_name : public MGHTestFunction_arbitrary_nm \
  { \
   public: \
    class_name(int n, int m, Function::DerivEvalType dEvalType) : \
      MGHTestFunction_arbitrary_nm(eval_name(n, m), dEvalType) { } \
    bool checkDimensions(int n, int m) const; \
  }

FUNCTION    (PowellBadlyScaled, PowellBadlyScaled_evaluator);
FUNCTION    (BrownBadlyScaled, BrownBadlyScaled_evaluator);
FUNCTION    (Beale, Beale_evaluator);
FUNCTION    (HelicalValley, HelicalValley_evaluator);
FUNCTION    (Gaussian, Gaussian_evaluator);
N_FUNCTION  (ExtendedRosenbrock, ExtendedRosenbrock_evaluator);
M_FUNCTION  (Gulf, Gulf_evaluator);
M_FUNCTION  (Box, Box_evaluator);
FUNCTION    (Wood, Wood_evaluator);
M_FUNCTION  (BrownDennis, BrownDennis_evaluator);
M_FUNCTION  (BiggsEXP6, BiggsEXP6_evaluator);
N_FUNCTION  (Watson, Watson_evaluator);
N_FUNCTION  (ExtendedPowellSingular, ExtendedPowellSingular_evaluator);
N_FUNCTION  (PenaltyFunctionI, PenaltyFunctionI_evaluator);
N_FUNCTION  (PenaltyFunctionII, PenaltyFunctionII_evaluator);
N_FUNCTION  (VariablyDimensioned, VariablyDimensioned_evaluator);
N_FUNCTION  (Trigonometric, Trigonometric_evaluator);
NM_FUNCTION (ChebyQuad, ChebyQuad_evaluator);

#undef FUNCTION
#undef N_FUNCTION
#undef M_FUNCTION
#undef NM_FUNCTION

#undef FUNCTION_EVALUATOR
#undef N_FUNCTION_EVALUATOR
#undef M_FUNCTION_EVALUATOR
#undef NM_FUNCTION_EVALUATOR

#undef FUNCTION

// TODO: Compute derivatives by hand and write gradient 
// and Hessian evaluators.

#define MGHTESTFUNCTION_H

#endif
