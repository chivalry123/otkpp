
#ifndef OTKGUIMODEL_H

#include <string>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

class BoundConstraints;
class Function;
class OTKGui;
class Solver;
class StoppingCriterion;

class OTKGuiModel
{
 public:
  enum ObjFuncType
  {
    USER_SYM_EXPR,
    PRED_TEST_FUNCTION
  };
  
  enum PlotType
  {
    CONTOUR,
    SURFACE
  };
  
  enum SelectedTool
  {
    PAN,
    PICK_CONSTRAINTS,
    PICK_ITER_PATH,
    PICK_ITERATE,
    PICK_SEARCH_LINE,
    PICK_STARTING_POINT,
    ZOOM_IN
  };
  
  OTKGuiModel();
  ~OTKGuiModel();
  
  void deactivateConstraints();
  const BoundConstraints *getConstraints() const;
  Function &getObjFunc();
  const Function &getObjFunc() const;
  ObjFuncType getObjFuncType() const;
  PlotType getPlotType() const;
  SelectedTool getSelectedTool() const;
  static Solver *getSolverInstance_(int index);
  double getStartingPointX() const;
  double getStartingPointY() const;
  const StoppingCriterion &getStopCrit() const;
  StoppingCriterion *getStopCritInstance(int index, double eps) const;
 /*signals:
  void minimizationResultsChanged(const Solver &);
  void objectiveFunctionChanged(const Function *);*/
 //public slots:
  void setConstraints(double x1, double x2, double y1, double y2);
#ifdef WITH_LIBMATHEVAL
  Function *setObjFunc(const std::string &expr);
#endif
  Function *setObjFunc(int tfIndex);
  void setObjFuncM(int m);
  void setObjFuncType(ObjFuncType type);
  //void setPlotSpec(const PlotSpec &plotSpec);
  void setPlotType(PlotType plotType);
  void setSelectedTool(SelectedTool t);
  void setStartingPointX(double x);
  void setStartingPointY(double y);
  void setStopCrit(const StoppingCriterion *stopCrit);
 private:
  BoundConstraints *C_;
  Function *objFunc_;
  int objFuncM_;
  ObjFuncType objFuncType_;
  PlotType plotType_;
  SelectedTool selectedTool_;
  double startingPointX_;
  double startingPointY_;
  const StoppingCriterion *stopCrit_;
  vector< double > x0_;
};

#define OTKGUIMODEL_H

#endif
