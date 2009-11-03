
#include "otkguimodel.h"

#include "BoundConstraints.h"
#include "ConjGradMT.h"
#include "DoglegBFGS.h"
#include "Function.h"
#include "GradNormTest.h"
#ifdef WITH_GSL
#include "GSLFSolver.h"
#include "GSLFDFSolver.h"
#endif
#include "HookeJeeves.h"
#ifdef WITH_FORTRAN
#include "LBFGSB.h"
#include "LMBM.h"
#endif
#include "LinminBFGS.h"
#include "MGHTestFunction.h"
#include "MNewton.h"
#include "PARTAN.h"
#include "SteihaugSR1.h"

#include <typeinfo>

OTKGuiModel::OTKGuiModel() :
  C_(NULL),
  objFunc_(NULL), objFuncM_(0),
  objFuncType_(USER_SYM_EXPR),
  selectedTool_(PICK_STARTING_POINT),
  stopCrit_(new GradNormTest(1e-8)) { }

OTKGuiModel::~OTKGuiModel()
{
  delete C_;
  delete objFunc_;
}

void OTKGuiModel::deactivateConstraints()
{
  delete C_;
  C_ = NULL;
}

const BoundConstraints *OTKGuiModel::getConstraints() const
{
  return C_;
}

Function *OTKGuiModel::getObjFunc()
{
  return objFunc_;
}

const Function *OTKGuiModel::getObjFunc() const
{
  return objFunc_;
}

OTKGuiModel::ObjFuncType OTKGuiModel::getObjFuncType() const
{
  return objFuncType_;
}

OTKGuiModel::PlotType OTKGuiModel::getPlotType() const
{
  return plotType_;
}

OTKGuiModel::SelectedTool OTKGuiModel::getSelectedTool() const
{
  return selectedTool_;
}

Solver *OTKGuiModel::getSolverInstance_(int index)
{
  int i = 0;
  
  Function::DerivEvalType derivEvalType;

#ifdef WITH_LIBMATHEVAL
  if(objFunc_->hasSymbolicExpression())
    derivEvalType = Function::DERIV_SYMBOLIC;
  else
#endif
    derivEvalType = Function::DERIV_FDIFF_CENTRAL_2;
  
  if(index == 0)
    return new MNewton();
  else if(index == 1)
    return new HookeJeeves();
  else if(index == 2)
    return new PARTAN();
  else if(index == 3)
    return new DoglegBFGS();
  else if(index == 4)
    return new SteihaugSR1();
  else if(index == 5)
    return new LinminBFGS(LinminBFGS::MORE_THUENTE);
  else if(index == 6)
    return new LinminBFGS(LinminBFGS::MORE_THUENTE, 5);
  else if(index == 7)
    return new ConjGradMT(ConjGradMT::FLETCHER_REEVES);
  else if(index == 8)
    return new ConjGradMT(ConjGradMT::POLAK_RIBIERE);
#ifdef WITH_GSL
  i = 6;
  if(index == 9)
    return new GSLFDFSolver(gsl_multimin_fdfminimizer_conjugate_fr, derivEvalType);
  else if(index == 10)
    return new GSLFDFSolver(gsl_multimin_fdfminimizer_conjugate_pr, derivEvalType);
  else if(index == 11)
    return new GSLFSolver(gsl_multimin_fminimizer_nmsimplex);
  else if(index == 12)
    return new GSLFDFSolver(gsl_multimin_fdfminimizer_steepest_descent, derivEvalType);
  else if(index == 13)
    return new GSLFDFSolver(gsl_multimin_fdfminimizer_vector_bfgs, derivEvalType);
  else if(index == 14)
    return new GSLFDFSolver(gsl_multimin_fdfminimizer_vector_bfgs2, derivEvalType);
#endif
#ifdef WITH_FORTRAN
  else if(index == 9 + i)
    return new LMBM();
  else if(index == 10 + i)
    return new LBFGSB(derivEvalType);
#endif
  else
    return NULL;
}

double OTKGuiModel::getStartingPointX() const
{
  return startingPointX_;
}

double OTKGuiModel::getStartingPointY() const
{
  return startingPointY_;
}

const StoppingCriterion &OTKGuiModel::getStopCrit() const
{
  return *stopCrit_;
}

StoppingCriterion *OTKGuiModel::getStopCritInstance(int index, double eps) const
{
  return new GradNormTest(eps);
}

void OTKGuiModel::setConstraints(double x1, double x2, double y1, double y2)
{
  delete C_;
  C_ = new BoundConstraints(2);
  C_->types[0] = BoundConstraints::BOTH;
  C_->types[1] = BoundConstraints::BOTH;
  C_->L[0] = x1;
  C_->L[1] = y1;
  C_->U[0] = x2;
  C_->U[1] = y2;
}

#ifdef WITH_LIBMATHEVAL
Function *OTKGuiModel::setObjFunc(const std::string &expr)
{
  /*delete objFunc_;
  objFunc_ = NULL;*/
  Function *oldObjFunc = objFunc_;
  objFunc_ = new Function(expr);
  return oldObjFunc;
}
#endif

Function *OTKGuiModel::setObjFunc(int tfIndex)
{
  /*delete objFunc_;
  objFunc_ = NULL;*/
  Function *oldObjFunc = objFunc_;
  
  if(tfIndex == 0)
    objFunc_ = new PowellBadlyScaled(Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 1)
    objFunc_ = new BrownBadlyScaled(Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 2)
    objFunc_ = new Beale(Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 3)
    objFunc_ = new Watson(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 4)
    objFunc_ = new ExtendedRosenbrock(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 5)
    objFunc_ = new PenaltyFunctionI(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 6)
    objFunc_ = new PenaltyFunctionII(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 7)
    objFunc_ = new VariablyDimensioned(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 8)
    objFunc_ = new Trigonometric(2, Function::DERIV_FDIFF_CENTRAL_2);
  else if(tfIndex == 9)
    objFunc_ = new ChebyQuad(2, objFuncM_, Function::DERIV_FDIFF_CENTRAL_2);
  
  if(dynamic_cast< MGHTestFunction_arbitrary_m * >(objFunc_) != NULL)
  {
    MGHTestFunction_arbitrary_m *m_func = 
      dynamic_cast< MGHTestFunction_arbitrary_m * >(objFunc_);
    if(!m_func->checkDimensions(objFuncM_))
    {
      /*delete objFunc_;
      objFunc_ = NULL;*/
      delete objFunc_;
      objFunc_ = oldObjFunc;
      throw std::invalid_argument("Invalid m parameter for test function.");
    }
  }
  else if(dynamic_cast< MGHTestFunction_arbitrary_nm * >(objFunc_) != NULL)
  {
    MGHTestFunction_arbitrary_nm *nm_func = 
      dynamic_cast< MGHTestFunction_arbitrary_nm * >(objFunc_);
    if(!nm_func->checkDimensions(2, objFuncM_))
    {
      /*delete objFunc_;
      objFunc_ = NULL;*/
      delete objFunc_;
      objFunc_ = oldObjFunc;
      throw std::invalid_argument("Invalid m parameter for test function.");
    }
  }
  
  return oldObjFunc;
}

void OTKGuiModel::setObjFuncM(int m)
{
  objFuncM_ = m;
}

void OTKGuiModel::setObjFuncType(OTKGuiModel::ObjFuncType type)
{
  objFuncType_ = type;
}

/*void OTKGuiModel::setPlotSpec(const PlotSpec &plotSpec)
{
  plotSpec_ = plotSpec;
}*/

void OTKGuiModel::setPlotType(PlotType plotType)
{
  plotType_ = plotType;
}

void OTKGuiModel::setSelectedTool(SelectedTool t)
{
  selectedTool_ = t;
}

void OTKGuiModel::setStartingPointX(double x)
{
  startingPointX_ = x;
}

void OTKGuiModel::setStartingPointY(double y)
{
  startingPointY_ = y;
}

void OTKGuiModel::setStopCrit(const StoppingCriterion *stopCrit)
{
  stopCrit_ = stopCrit;
}
