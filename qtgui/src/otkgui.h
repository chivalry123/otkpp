
#ifndef OTKGUI_H
#define OTKGUI_H

#include "minimizerthread.h"
#include "ui_mainwindow.h"

#include <Solver.h>
#include <StoppingCriterion.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <QtGui>

class MinimizerThread;
class OTKGuiApplication;
class OTKGuiModel;

class OTKGui : public QMainWindow
{
  Q_OBJECT
 public:
  OTKGui(OTKGuiApplication *application, OTKGuiModel *model);
  ~OTKGui();
  
  Ui_MainWindow *getWidgets();
  void showMinimizationResults(const MinimizerThread::Results &results);
 private slots:
  void about();
  void clearIterates();
  void deactivateConstraints();
  void deselectIterate();
  void deselectIterPath();
  void deselectSearchLine();
  void panPlot();
  void removeSelectedIterPath();
  void selectContourPlot();
  void selectPanTool();
  void selectPickConstraintsTool();
  void selectPickIterateTool();
  void selectPickIterPathTool();
  void selectPickSearchLineTool();
  void selectPickStartingPointTool();
  void selectSurfacePlot();
  void selectZoomInTool();
  void selectSymbolicFunction();
  void selectPredTestFunction();
  void setConstraints(double x1, double x2, double y1, double y2);
  void setHold(int checked);
  void setLinearZAxis();
  void setLog10ZAxis();
  void setStartingPoint(double x, double y);
  void savePlot();
  void showMinimizationResults();
  void showSelectedIterate(double x, double y);
  void showSelectedLine(double x1, double y1, double x2, double y2);
  void startIteration();
  void stopIteration();
  void updateConstraints();
  void updateObjectiveFunction();
  void updatePlotRangesFromTextFields();
  void updateRangeFieldsFromPlot();
  void zoomOut();
 private:
  static const double INIT_X1;
  static const double INIT_X2;
  static const double INIT_Y1;
  static const double INIT_Y2;
  static const double INIT_Z1;
  static const double INIT_Z2;
  
  OTKGuiApplication *application_;
  OTKGuiModel *guiModel_;
  MinimizerThread *minimizerThread_;
  QButtonGroup plotTypeButtonGroup_;
  QButtonGroup scaleTypeButtonGroup_;
  QButtonGroup toolButtonGroup_;
  Ui_MainWindow *widgets_;
  
  void getTFPlotRange_(int tfIndex,
                       double &x1, double &x2,
                       double &y1, double &y2,
                       double &z1, double &z2,
                       bool &zLogScale);
  StoppingCriterion *getStopCritInstance_();
  void initPredTestFuncComboBox_();
  void initSolverComboBox_();
  void initStopCritComboBox_();
  void setPlotRange_(double x1, double x2, 
                     double y1, double y2);
  void setPlotRange_(double x1, double x2, 
                     double y1, double y2, 
                     double z1, double z2);
};

#endif
