
#include "CompiledFunctionEvaluator.h"
#include "FDiffGradientEvaluator.h"
#include "FDiffHessianEvaluator.h"
#include "Function.h"
#ifdef WITH_LIBMATHEVAL
#include "SymbolicFunctionEvaluator.h"
#include "SymbolicGradientEvaluator.h"
#include "SymbolicHessianEvaluator.h"
#endif

#include <sstream>

Function::Function()
{
  evaluator_  = NULL;
  gEvaluator_ = NULL;
  HEvaluator_ = NULL;
}

#ifdef WITH_LIBMATHEVAL
Function::Function(const std::string &expr,
                   DerivEvalType gEvalType)
{
  constructSymbolicFunction_(expr, gEvalType);
}
#endif

Function::Function(const FunctionEvaluator &fEval,
                   DerivEvalType gEvalType)
{
  evaluator_ = fEval.clone();
  
#ifdef WITH_LIBMATHEVAL
  if(gEvalType == Function::DERIV_SYMBOLIC)
  {
    if(dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_) != NULL)
    {
      gEvaluator_ = new SymbolicGradientEvaluator(
        dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_));
      HEvaluator_ = new SymbolicHessianEvaluator(
          dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_),
          dynamic_cast< SymbolicGradientEvaluator * >(gEvaluator_));
    }
    else
      throw std::runtime_error("symbolic gradient evaluator requires a symbolic function evaluator");
  }
  else
#endif
  if(gEvalType == Function::DERIV_FDIFF_CENTRAL_2)
  {
    gEvaluator_ = new FDiffGradientEvaluator(FDiffGradientEvaluator::CENTRAL_2, evaluator_);
    HEvaluator_ = new FDiffHessianEvaluator(evaluator_);
  }
  else
    throw std::runtime_error("unsupported gradient evaluator type");
}

Function::Function(const Function &f)
{
  evaluator_  = f.evaluator_->clone();
  gEvaluator_ = f.gEvaluator_->clone();
  if(gEvaluator_->usesFiniteDifference())
    dynamic_cast< FDiffGradientEvaluator * >(gEvaluator_)->setFEvaluator(evaluator_);
  HEvaluator_ = f.HEvaluator_->clone();
  // TODO: do this to Hessian also
  /*if(gEvaluator_->usesFiniteDifference())
    dynamic_cast< FDiffGradientEvaluator * >(gEvaluator_)->setFEvaluator(evaluator_);*/
}

Function::~Function()
{
  delete HEvaluator_;
  delete gEvaluator_;
  delete evaluator_;
}

Function &Function::operator=(const Function &f)
{
  Function f_(f);
  
  FunctionEvaluator *oldEval = evaluator_;
  evaluator_  = f_.evaluator_;
  GradientEvaluator *oldGEval = gEvaluator_;
  gEvaluator_ = f_.gEvaluator_;
  HessianEvaluator *oldHEval = HEvaluator_;
  HEvaluator_ = f_.HEvaluator_;
  
  f_.evaluator_  = oldEval;
  f_.gEvaluator_ = oldGEval;
  f_.HEvaluator_ = oldHEval;
  
  return *this;
}

double Function::operator()(const double *x) const
{
  return (*evaluator_)(x);
}

double Function::operator()(const vector< double > &x) const
{
  return (*evaluator_)(x);
}

#ifdef WITH_LIBMATHEVAL
void Function::constructSymbolicFunction_(const std::string &expr, 
                                          DerivEvalType gEvalType)
{
  evaluator_  = new SymbolicFunctionEvaluator(expr);
  if(gEvalType == Function::DERIV_SYMBOLIC)
  {
    gEvaluator_ = new SymbolicGradientEvaluator(
      dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_));
    HEvaluator_ = new SymbolicHessianEvaluator(
      dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_),
      dynamic_cast< SymbolicGradientEvaluator * >(gEvaluator_));
  }
  else if(gEvalType == Function::DERIV_FDIFF_CENTRAL_2)
  {
    gEvaluator_ = new FDiffGradientEvaluator(FDiffGradientEvaluator::CENTRAL_2, evaluator_);
    HEvaluator_ = new FDiffHessianEvaluator(evaluator_);
  }
  else
    throw std::runtime_error("unsupported gradient evaluator type");
}
#endif

double *Function::g(const double *x, double *g) const
{
  gEvaluator_->eval(x, g);
  return g;
}

vector< double > &Function::g(const vector< double > &x, 
                              vector< double > &g) const
{
  gEvaluator_->eval(x, g);
  return g;
}

matrix< double > &Function::H(const vector< double > &x, 
                              matrix< double > &H) const
{
  HEvaluator_->eval(x, H);
  return H;
}

Function Function::createCopy(DerivEvalType gEvalType) const
{
  Function f;
  f.evaluator_ = evaluator_->clone();
#ifdef WITH_LIBMATHEVAL
  if(gEvalType == Function::DERIV_SYMBOLIC)
    f.gEvaluator_ = new SymbolicGradientEvaluator(
      dynamic_cast< SymbolicFunctionEvaluator * >(f.evaluator_));
  else
#endif
  if(gEvalType == Function::DERIV_FDIFF_CENTRAL_2)
    f.gEvaluator_ = new FDiffGradientEvaluator(
      FDiffGradientEvaluator::CENTRAL_2, f.evaluator_);
  else
    throw std::runtime_error("unsupported gradient evaluator type");
  f.HEvaluator_ = new FDiffHessianEvaluator(f.evaluator_);
  
  return f;
}

int Function::getN() const
{
  return evaluator_->getN();
}

#ifdef WITH_LIBMATHEVAL
const std::string Function::getSymbolicExpression() const
{
  if(hasSymbolicExpression())
    return dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_)->getExpression();
}
#endif

void Function::getVariableNames(std::list< std::string > &result) const
{
#ifdef WITH_LIBMATHEVAL
  if(hasSymbolicExpression())
  {
    result.clear();
    const char **vn = dynamic_cast< 
      SymbolicFunctionEvaluator * >(evaluator_)->getVarNames();
    for(int i = 0; i < getN(); i++)
      result.push_back(vn[i]);
  }
  else
  {
#endif
    result.clear();
    for(int i = 0; i < getN(); i++)
    {
      std::ostringstream vn;
      vn<<"x"<<i+1;
      result.push_back(vn.str());
    }
#ifdef WITH_LIBMATHEVAL
  }
#endif
}

bool Function::hasSymbolicExpression() const
{
#ifdef WITH_LIBMATHEVAL
  if(dynamic_cast< SymbolicFunctionEvaluator * >(evaluator_) != NULL)
    return true;
  else
#endif
    return false;
}

int Function::getFuncEvalCounter() const
{
  return evaluator_->getEvalCounter();
}

int Function::getGradEvalCounter() const
{
  return gEvaluator_->getEvalCounter();
}

int Function::getHessEvalCounter() const
{
  return HEvaluator_->getEvalCounter();
}

void Function::enableEvalCounting() const
{
  evaluator_->enableEvalCounting();
  gEvaluator_->enableEvalCounting();
  //HEvaluator_->enableEvalCounting();
}

void Function::disableEvalCounting() const
{
  evaluator_->disableEvalCounting();
  gEvaluator_->disableEvalCounting();
  //HEvaluator_->disableEvalCounting();
}

void Function::resetEvalCounters()
{
  evaluator_->resetEvalCounter();
  gEvaluator_->resetEvalCounter();
  //HEvaluator_->resetEvalCounter();
}
