
#include "minimizerthread.h"
#include "otkgui.h"
#include "otkguiapplication.h"
#include "otkguimodel.h"

#include <OTK.h>

#include <QPrinter>
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>
#include <qwt_event_pattern.h>
#include <sstream>

const double OTKGui::INIT_X1 = -2.0;
const double OTKGui::INIT_X2 = 2.0;
const double OTKGui::INIT_Y1 = -2.0;
const double OTKGui::INIT_Y2 = 2.0;
const double OTKGui::INIT_Z1 = 1e-6;
const double OTKGui::INIT_Z2 = 1000;

OTKGui::OTKGui(OTKGuiApplication *application, OTKGuiModel *model)
{
  application_ = application;
  guiModel_ = model;
  minimizerThread_ = NULL;
  
  widgets_ = new Ui_MainWindow();
  widgets_->setupUi(this);
  
  initPredTestFuncComboBox_();
  initSolverComboBox_();
  initStopCritComboBox_();
  
  plotTypeButtonGroup_.addButton(widgets_->contourPlotSelectionButton);
  plotTypeButtonGroup_.addButton(widgets_->surfacePlotSelectionButton);
  widgets_->contourPlotSelectionButton->setChecked(true);
  
  toolButtonGroup_.addButton(widgets_->selectPanButton);
  toolButtonGroup_.addButton(widgets_->selectZoomInButton);
  
  toolButtonGroup_.addButton(widgets_->pickConstraintsButton);
  toolButtonGroup_.addButton(widgets_->pickIterateButton);
  toolButtonGroup_.addButton(widgets_->pickIterPathButton);
  toolButtonGroup_.addButton(widgets_->pickSearchLineButton);
  toolButtonGroup_.addButton(widgets_->pickStartingPointButton);
  
  scaleTypeButtonGroup_.addButton(widgets_->linearScaleSelectionButton);
  scaleTypeButtonGroup_.addButton(widgets_->log10ScaleSelectionButton);
  widgets_->log10ScaleSelectionButton->setChecked(true);
  
  widgets_->iterationPlot->setPlotRange(INIT_X1, INIT_X2,
                                        INIT_Y1, INIT_Y2,
                                        INIT_Z1, INIT_Z2);
  
  widgets_->surfacePlot->setPlotRange(INIT_X1, INIT_X2,
                                      INIT_Y1, INIT_Y2,
                                      INIT_Z1, INIT_Z2);
  
  setPlotRange_(INIT_X1, INIT_X2,
                INIT_Y1, INIT_Y2,
                INIT_Z1, INIT_Z2);
  
  guiModel_->setPlotType(OTKGuiModel::CONTOUR);
  widgets_->surfacePlot->setVisible(false);
}

OTKGui::~OTKGui() { }

void OTKGui::about()
{
  QMessageBox::about(this, tr("About Application"), tr("This example demonstrates..."));
}

void OTKGui::clearIterates()
{
  widgets_->iterationPlot->clearIterates();
}

void OTKGui::deactivateConstraints()
{
  guiModel_->deactivateConstraints();
  widgets_->iterationPlot->deactivateConstraints();
}

void OTKGui::deselectIterate()
{
  widgets_->iterationPlot->clearSelections();
}

void OTKGui::deselectIterPath()
{
  widgets_->iterationPlot->clearSelections();
}

void OTKGui::deselectSearchLine()
{
  widgets_->iterationPlot->clearSelections();
}

void OTKGui::panPlot()
{
  QwtScaleMap xScaleMap = widgets_->iterationPlot->canvasMap(QwtPlot::xBottom);
  QwtScaleMap yScaleMap = widgets_->iterationPlot->canvasMap(QwtPlot::yLeft);
  
  setPlotRange_(xScaleMap.s1(), xScaleMap.s2(),
                yScaleMap.s1(), yScaleMap.s2());
}

void OTKGui::removeSelectedIterPath()
{
  widgets_->iterationPlot->removeSelectedIterPath();
}

Ui_MainWindow *OTKGui::getWidgets()
{
  return widgets_;
}

void OTKGui::getTFPlotRange_(int tfIndex,
                             double &x1, double &x2,
                             double &y1, double &y2,
                             double &z1, double &z2,
                             bool &zLogScale)
{
  if(tfIndex == 0)
  {
    x1 = -5e-5;
    x2 = 2e-4;
    y1 = -1.0;
    y2 = 10.0;
    z1 = 1e-6;
    z2 = 1000.0;
    zLogScale = true;
  }
  else if(tfIndex == 1)
  {
    x1 = 1e6-1e-4;
    x2 = 1e6+1e-4;
    y1 = 2e-6-1e-10;
    y2 = 2e-6+1e-10;
    z1 = 1e-16;
    z2 = 1e-6;
    zLogScale = true;
  }
  else if(tfIndex == 2)
  {
    x1 = 0.25;
    x2 = 4.25;
    y1 = -1.0;
    y2 = 1.5;
    z1 = 1e-3;
    z2 = 500.0;
    zLogScale = true;
  }
  else if(tfIndex == 3)
  {
    x1 = -1.5;
    x2 = 0.4;
    y1 = 0.0;
    y2 = 2.0;
    z1 = 0.25;
    z2 = 500.0;
    zLogScale = true;
  }
  else if(tfIndex == 4)
  {
    x1 = -1.5;
    x2 = 1.4;
    y1 = -0.25;
    y2 = 1.25;
    z1 = 1e-3;
    z2 = 1000.0;
    zLogScale = true;
  }
  else if(tfIndex == 5)
  {
    x1 = -1.0;
    x2 = 1.0;
    y1 = -1.0;
    y2 = 1.0;
    z1 = 1e-6;
    z2 = 10.0;
    zLogScale = true;
  }
  else if(tfIndex == 6)
  {
    x1 = -1.0;
    x2 = 1.25;
    y1 = -2.0;
    y2 = 2.0;
    z1 = 1e-4;
    z2 = 50.0;
    zLogScale = true;
  }
  else if(tfIndex == 7)
  {
    x1 = 0.0;
    x2 = 2.0;
    y1 = 0.0;
    y2 = 2.0;
    z1 = 1e-4;
    z2 = 100.0;
    zLogScale = true;
  }
  else if(tfIndex == 8)
  {
    x1 = -10.0;
    x2 = 10.0;
    y1 = -10.0;
    y2 = 10.0;
    z1 = 1e-5;
    z2 = 140.0;
    zLogScale = false;
  }
  else if(tfIndex == 9)
  {
    x1 = 0.0;
    x2 = 1.0;
    y1 = 0.0;
    y2 = 1.0;
    z1 = 1.0;
    z2 = 10.0;
    zLogScale = false;
  }
  else
    throw std::invalid_argument("invalid test function index");
}

StoppingCriterion *OTKGui::getStopCritInstance_()
{
  std::string epsStr = widgets_->epsField->displayText().toStdString();
  bool ok;
  double eps = widgets_->epsField->displayText().toDouble(&ok);
  if(!ok)
  {
    QMessageBox::critical(this, tr("Error"), tr("Invalid epsilon value."));
    return NULL;
  }
  
  int index = widgets_->stopCritComboBox->currentIndex();
  return guiModel_->getStopCritInstance(index, eps);
}

void OTKGui::initPredTestFuncComboBox_()
{
  widgets_->funcNameComboBox->insertItem(0, "Powell badly scaled");
  widgets_->funcNameComboBox->insertItem(1, "Brown badly scaled");
  widgets_->funcNameComboBox->insertItem(2, "Beale");
  widgets_->funcNameComboBox->insertItem(3, "Watson");
  widgets_->funcNameComboBox->insertItem(4, "Extended Rosenbrock");
  widgets_->funcNameComboBox->insertItem(5, "Penalty Function I");
  widgets_->funcNameComboBox->insertItem(6, "Penalty Function II");
  widgets_->funcNameComboBox->insertItem(7, "Variably dimensioned");
  widgets_->funcNameComboBox->insertItem(8, "Trigonometric");
  widgets_->funcNameComboBox->insertItem(9, "Chebyquad");
}

void OTKGui::initSolverComboBox_()
{
  widgets_->algorithmComboBox->insertItem(0,  "MNewton");
  widgets_->algorithmComboBox->insertItem(1,  "HookeJeeves");
  widgets_->algorithmComboBox->insertItem(2,  "PARTAN");
  widgets_->algorithmComboBox->insertItem(3,  "BFGS/dogleg");
  widgets_->algorithmComboBox->insertItem(4,  "SR1/Steihaug");
  widgets_->algorithmComboBox->insertItem(5,  "BFGS/MT");
  widgets_->algorithmComboBox->insertItem(6,  "LBFGS/MT");
  widgets_->algorithmComboBox->insertItem(7,  "ConjGrad-FR/MT");
  widgets_->algorithmComboBox->insertItem(8,  "ConjGrad-PR/MT");
#ifdef WITH_GSL
  widgets_->algorithmComboBox->insertItem(9,  "gsl_conjugate_fr");
  widgets_->algorithmComboBox->insertItem(10, "gsl_conjugate_pr");
  widgets_->algorithmComboBox->insertItem(11, "gsl_nmsimplex");
  widgets_->algorithmComboBox->insertItem(12, "gsl_steepest_descent");
  widgets_->algorithmComboBox->insertItem(13, "gsl_vector_bfgs");
  widgets_->algorithmComboBox->insertItem(14, "gsl_vector_bfgs2");
#endif
#ifdef WITH_FORTRAN
  widgets_->algorithmComboBox->insertItem(15, "LMBM");
  widgets_->algorithmComboBox->insertItem(16, "L-BFGS-B");
#endif
}

/*void OTKGui::initDiffComboBoxes_()
{
  widgets_->gradDiffComboBox->insertItem(0, "symbolic");
  widgets_->gradDiffComboBox->insertItem(1, "forward difference");
  widgets_->gradDiffComboBox->insertItem(2, "central difference");
  
  widgets_->hessDiffComboBox->insertItem(0, "symbolic");
  widgets_->hessDiffComboBox->insertItem(1, "forward difference");
  widgets_->hessDiffComboBox->insertItem(2, "central difference");
}*/

void OTKGui::initStopCritComboBox_()
{
  widgets_->stopCritComboBox->insertItem(1, "|nabla f| < eps");
}

void OTKGui::selectPanTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PAN);
  widgets_->iterationPlot->selectPanTool();
}

void OTKGui::selectPickConstraintsTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PICK_CONSTRAINTS);
  widgets_->iterationPlot->selectPickTool(IterationPlot::PICK_CONSTRAINTS);
}

void OTKGui::selectPickIterateTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PICK_ITERATE);
  widgets_->iterationPlot->selectPickTool(IterationPlot::PICK_ITERATE);
}

void OTKGui::selectPickIterPathTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PICK_ITER_PATH);
  widgets_->iterationPlot->selectPickTool(IterationPlot::PICK_ITER_PATH);
}

void OTKGui::selectPickSearchLineTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PICK_SEARCH_LINE);
  widgets_->iterationPlot->selectPickTool(IterationPlot::PICK_SEARCH_LINE);
}

void OTKGui::selectPickStartingPointTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::PICK_STARTING_POINT);
  widgets_->iterationPlot->selectPickTool(IterationPlot::PICK_COORDINATES);
}

void OTKGui::selectContourPlot()
{
  guiModel_->setPlotType(OTKGuiModel::CONTOUR);
  widgets_->surfacePlot->setVisible(false);
  widgets_->iterationPlot->setVisible(true);
}

void OTKGui::selectZoomInTool()
{
  guiModel_->setSelectedTool(OTKGuiModel::ZOOM_IN);
  widgets_->iterationPlot->selectZoomInTool();
}

void OTKGui::setConstraints(double x1, double x2, double y1, double y2)
{
  widgets_->constraintX1Field->setText(QString::number(x1, 'g', 3));
  widgets_->constraintX2Field->setText(QString::number(x2, 'g', 3));
  widgets_->constraintY1Field->setText(QString::number(y1, 'g', 3));
  widgets_->constraintY2Field->setText(QString::number(y2, 'g', 3));
  
  guiModel_->setConstraints(x1, x2, y1, y2);
}

void OTKGui::startIteration()
{
  if(minimizerThread_ != NULL)
    return;
  
  if(guiModel_->getObjFunc() == NULL)
  {
    QMessageBox::critical(this, tr("Error"), tr("No objective function specified."));
    return;
  }
  
  int solverIndex = widgets_->algorithmComboBox->currentIndex();
  Solver *solver = guiModel_->getSolverInstance_(solverIndex);
  StoppingCriterion *stopCrit = getStopCritInstance_();
  if(stopCrit == NULL)
  {
    delete solver;
    return;
  }
  
  guiModel_->getObjFunc()->resetEvalCounters();
  guiModel_->getObjFunc()->enableEvalCounting();
  
  delete minimizerThread_;
  minimizerThread_ = 
    new MinimizerThread(guiModel_->getObjFunc(), solver,
                        guiModel_->getStartingPointX(),
                        guiModel_->getStartingPointY(),
                        stopCrit,
                        guiModel_->getConstraints());
  setCursor(Qt::WaitCursor);
  
  connect(minimizerThread_, SIGNAL(finished()), this, SLOT(showMinimizationResults()));
  minimizerThread_->start(QThread::TimeCriticalPriority);
}

void OTKGui::stopIteration()
{
  if(minimizerThread_ != NULL)
    minimizerThread_->raiseTerminationFlag();
}

void OTKGui::selectSurfacePlot()
{
  IterationPlot *ip = widgets_->iterationPlot;
  SurfacePlot *sp = widgets_->surfacePlot;
  
  guiModel_->setPlotType(OTKGuiModel::SURFACE);
  ip->setVisible(false);
  sp->setVisible(true);
  
  sp->setPlotRange(ip->getX1(), ip->getX2(), 
                   ip->getY1(), ip->getY2(), 
                   ip->getZ1(), ip->getZ2());
}

void OTKGui::selectSymbolicFunction()
{
  widgets_->funcNameComboBox->setEnabled(false);
  widgets_->testFuncMField->setEnabled(false);
  
  widgets_->objFuncExpression->setEnabled(true);
  widgets_->functionUpdateButton->setEnabled(true);
  
  guiModel_->setObjFuncType(OTKGuiModel::USER_SYM_EXPR);
}

void OTKGui::selectPredTestFunction()
{
  widgets_->objFuncExpression->setEnabled(false);
  
  widgets_->funcNameComboBox->setEnabled(true);
  widgets_->testFuncMField->setEnabled(true);
  
  guiModel_->setObjFuncType(OTKGuiModel::PRED_TEST_FUNCTION);
}

void OTKGui::setHold(int checked)
{
  if(checked == 0)
    widgets_->iterationPlot->setHold(false);
  else
    widgets_->iterationPlot->setHold(true);
}

void OTKGui::setLinearZAxis()
{
  widgets_->surfacePlot->setZAxisScaleType(Qwt3D::LINEARSCALE);
}

void OTKGui::setLog10ZAxis()
{
  widgets_->surfacePlot->setZAxisScaleType(Qwt3D::LOG10SCALE);
}

void OTKGui::showMinimizationResults(const MinimizerThread::Results &results)
{
  /*widgets_->minimizerX->setText(QString::number(solver.getX()[0], 'g', 3));
  widgets_->minimizerY->setText(QString::number(solver.getX()[1], 'g', 3));
  widgets_->minimumFValue->setText(QString::number(solver.getFVal(), 'g', 3));
  widgets_->numIter->setText(QString::number(solver.getNumIter()));
  widgets_->numFuncEval->setText(QString::number(solver.getNumFuncEval()));
  widgets_->numGradEval->setText(QString::number(solver.getNumGradEval()));*/
  widgets_->minimizerX->setText(QString::number(results.xMin, 'g', 3));
  widgets_->minimizerY->setText(QString::number(results.yMin, 'g', 3));
  widgets_->minimumFValue->setText(QString::number(results.fMin, 'g', 3));
  widgets_->numIter->setText(QString::number(results.numIter));
  widgets_->numFuncEval->setText(QString::number(results.numFuncEval));
  widgets_->numGradEval->setText(QString::number(results.numGradEval));
}

void OTKGui::setStartingPoint(double x, double y)
{
  guiModel_->setStartingPointX(x);
  guiModel_->setStartingPointY(y);
  
  widgets_->x0Field->setText(QString::number(x, 'g', 3));
  widgets_->y0Field->setText(QString::number(y, 'g', 3));
}

void OTKGui::setPlotRange_(double x1, double x2,
                           double y1, double y2)
{
  widgets_->xRangeStartField->setText(QString::number(x1, 'g', 3));
  widgets_->xRangeEndField->setText  (QString::number(x2, 'g', 3));
  
  widgets_->yRangeStartField->setText(QString::number(y1, 'g', 3));
  widgets_->yRangeEndField->setText  (QString::number(y2, 'g', 3));
}

void OTKGui::setPlotRange_(double x1, double x2,
                           double y1, double y2,
                           double z1, double z2)
{
  setPlotRange_(x1, x2, y1, y2);
  
  widgets_->zRangeStartField->setText(QString::number(z1, 'g', 3));
  widgets_->zRangeEndField->setText  (QString::number(z2, 'g', 3));
}

void OTKGui::savePlot()
{
  QPrinter printer;
  
  QString fileName = 
    QFileDialog::getSaveFileName(this, tr("Save plot as.."), QString(), "*.pdf");
  
  printer.setOrientation(QPrinter::Landscape);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(fileName);
  
  widgets_->iterationPlot->print(printer);
}

void OTKGui::showMinimizationResults()
{
  const MinimizerThread::Results &results = minimizerThread_->getResults();
  const std::list< vector< double > > &iterates = 
    minimizerThread_->getResults().iterates;
  const std::list< matrix< double > > *iterPointSets;
  
  if(results.iteratePointSets.size() > 0)
    iterPointSets = &results.iteratePointSets;
  else
    iterPointSets = NULL;
  
  if(!widgets_->iterationPlot->getHold())
    widgets_->iterationPlot->setIterationPath(iterates, iterPointSets);
  else
  {
    if(!widgets_->iterationPlot->insertIterationPath(iterates, iterPointSets))
    {
      std::ostringstream errStr;
      errStr<<"No more than "<<widgets_->iterationPlot->getMaxNumPlots()<<" iteration plots allowed.";
      QMessageBox::critical(this, tr("Error"), tr(errStr.str().c_str()));
    }
  }
  
  showMinimizationResults(results);
  
  minimizerThread_->deleteLater();
  minimizerThread_ = NULL;
  
  setCursor(Qt::ArrowCursor);
}

void OTKGui::showSelectedIterate(double x, double y)
{
  double fx;
  vector< double > g(2);
  double gNorm;
  vector< double > xv(2);
  
  widgets_->curIterXField->setText(QString::number(x, 'g', 3));
  widgets_->curIterYField->setText(QString::number(y, 'g', 3));

  xv(0) = x;
  xv(1) = y;
  fx = (*guiModel_->getObjFunc())(xv);
  widgets_->curIterFValField->setText(QString::number(fx, 'g', 3));
  
  guiModel_->getObjFunc()->g(xv, g);
  gNorm = norm_2(g);
  widgets_->curIterGNormField->setText(QString::number(gNorm, 'g', 3));
}

void OTKGui::showSelectedLine(double x1, double y1, double x2, double y2)
{
  widgets_->searchLinePlot->setSearchLine(x1, y1, x2, y2);
}

void OTKGui::updateObjectiveFunction()
{
  std::string fExpr = widgets_->objFuncExpression->displayText().toStdString();
  Function *oldObjFunc = NULL;
  
  if(guiModel_->getObjFuncType() == OTKGuiModel::USER_SYM_EXPR)
  {
#ifdef WITH_LIBMATHEVAL
    std::string expr = widgets_->objFuncExpression->displayText().toStdString();
    try {
      oldObjFunc = guiModel_->setObjFunc(expr);
    }
    catch(std::invalid_argument &e) {
      QMessageBox::critical(this, tr("Error"), tr("Invalid function expression. Check syntax."));
      return;
    }
    if(guiModel_->getObjFunc()->getN() != 2)
    {
      QMessageBox::critical(this, tr("Error"), tr("Invalid number of variables. Must be n=2."));
      return;
    }
#else
    QMessageBox::critical(this, tr("Error"), tr("OTK++ was not compiled with libmatheval support. Symbolic function expressions are not supported."));
#endif
  }
  else
  {
    bool ok;
    int m = widgets_->testFuncMField->displayText().toInt(&ok);
    guiModel_->setObjFuncM(m);
    int tfIndex = widgets_->funcNameComboBox->currentIndex();
    try {
      oldObjFunc = guiModel_->setObjFunc(tfIndex);
    }
    catch(std::invalid_argument &e) {
      QMessageBox::critical(this, tr("Error"), tr("Invalid m parameter for objective function."));
      return;
    }
    double x1, x2, y1, y2, z1, z2;
    bool zLogScale;
    getTFPlotRange_(tfIndex, x1, x2, y1, y2, z1, z2, zLogScale);
    widgets_->iterationPlot->setPlotRange(x1, x2, y1, y2, z1, z2);
    widgets_->surfacePlot->setPlotRange(x1, x2, y1, y2, z1, z2);
    setPlotRange_(x1, x2, y1, y2, z1, z2);
  }
  widgets_->iterationPlot->setFunction (guiModel_->getObjFunc());
  widgets_->searchLinePlot->setFunction(guiModel_->getObjFunc());
  widgets_->surfacePlot->setFunction   (guiModel_->getObjFunc());
  
  if(oldObjFunc != guiModel_->getObjFunc())
    delete oldObjFunc;
}

void OTKGui::updateConstraints()
{
  double x1 = widgets_->constraintX1Field->text().toDouble();
  double x2 = widgets_->constraintX2Field->text().toDouble();
  double y1 = widgets_->constraintY1Field->text().toDouble();
  double y2 = widgets_->constraintY2Field->text().toDouble();
  
  guiModel_->setConstraints(x1, x2, y1, y2);
  widgets_->iterationPlot->setConstraints(x1, x2, y1, y2);
}

void OTKGui::updatePlotRangesFromTextFields()
{
  double x1 = widgets_->xRangeStartField->text().toDouble();
  double x2 = widgets_->xRangeEndField->text().toDouble();
  double y1 = widgets_->yRangeStartField->text().toDouble();
  double y2 = widgets_->yRangeEndField->text().toDouble();
  double z1 = widgets_->zRangeStartField->text().toDouble();
  double z2 = widgets_->zRangeEndField->text().toDouble();
  
  widgets_->iterationPlot->setPlotRange(x1, x2, y1, y2, z1, z2);
  widgets_->surfacePlot->setPlotRange(x1, x2, y1, y2, z1, z2);
}

void OTKGui::updateRangeFieldsFromPlot()
{
  QwtScaleMap xScaleMap = widgets_->iterationPlot->canvasMap(QwtPlot::xBottom);
  QwtScaleMap yScaleMap = widgets_->iterationPlot->canvasMap(QwtPlot::yLeft);
  
  setPlotRange_(xScaleMap.s1(), xScaleMap.s2(), 
                yScaleMap.s1(), yScaleMap.s2());
}

void OTKGui::zoomOut()
{
  double x1 = widgets_->iterationPlot->getX1();
  double x2 = widgets_->iterationPlot->getX2();
  double y1 = widgets_->iterationPlot->getY1();
  double y2 = widgets_->iterationPlot->getY2();
  
  double xc = (x2 + x1) / 2.0;
  double yc = (y2 + y1) / 2.0;
  
  x1 = xc + (x1 - xc) * 1.1;
  x2 = xc + (x2 - xc) * 1.1;
  y1 = yc + (y1 - yc) * 1.1;
  y2 = yc + (y2 - yc) * 1.1;
  
  widgets_->iterationPlot->setPlotRange(x1, x2, y1, y2);
  setPlotRange_(x1, x2, y1, y2);
}
