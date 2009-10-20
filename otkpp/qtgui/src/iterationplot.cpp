
#include "contourdata.h"
#include "iterationplot.h"
#include "logarithmiccolormap.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <QMessageBox>
#include <QPainter>
#include <qwt_color_map.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>

const QColor IterationPlot::ITER_PATH_COLORS_[] = 
{
  Qt::green,
  Qt::blue,
  Qt::yellow,
  Qt::magenta,
  Qt::cyan,
};

const int IterationPlot::NUM_CONTOUR_LEVELS_ = 50;

static void logSpace(int nSteps, double xmin, double xmax, double *array)
{
  if (xmin <= 0.0 || xmax <= 0.0 || nSteps <= 0)
    return;
  
  const int I_MAX = nSteps - 1;
  
  array[0] = xmin;
  array[I_MAX] = xmax;
  
  const double lxmin = log10(xmin);
  const double lxmax = log10(xmax);
  const double lstep = (lxmax - lxmin) / double(I_MAX);
  
  for (int i = 1; i < I_MAX; i++)
    array[i] = pow(10.0, lxmin + double(i) * lstep);
}

IterationPlot::IterationPlot(QWidget *parent) : 
  QwtPlot(parent), hold_(false), iterPathSelected_(false), selectedIterPath_(NULL)
{
  for(int i = 0; i < NUM_ITER_PATH_COLORS_; i++)
    freeColorIndices_[i] = true;
  
  setAxisScale(QwtPlot::xBottom, -2.0, 2.0);
  setAxisScale(QwtPlot::yLeft, -2.0, 2.0);
  
  setCanvasBackground(QColor(255, 255, 255));
  
  QwtSymbol sym;
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setSize(10);
  sym.setBrush(QBrush(Qt::green));
  sym.setPen(QPen(Qt::green, 1));
  selectedStartingPointPlot_.setSymbol(sym);
  
  contourPlot_.setData(contourData_);
  
  //LogarithmicColorMap colorMap;
  //colorMap.setColorInterval(QColor(150, 150, 150), QColor(255, 255, 255));
  contourPlot_.setColorMap(LogarithmicColorMap());
  
  /*QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  rightAxis->setTitle("f(x)");
  rightAxis->setColorBarEnabled(true);
  rightAxis->setColorMap(contourPlot_.data().range(), contourPlot_.colorMap());
  setAxisScaleEngine(QwtPlot::yRight, new QwtLog10ScaleEngine);
  setAxisScale(QwtPlot::yRight, 1e-6, 1000);
  enableAxis(QwtPlot::yRight);*/
  
  contourPlot_.setDisplayMode(QwtPlotSpectrogram::ContourMode, true);
  contourPlot_.setDisplayMode(QwtPlotSpectrogram::ImageMode, false);
  
  //contourPlot_.setRenderHint(QwtPlotItem::RenderAntialiased);
  contourPlot_.setDefaultContourPen(QPen(QColor(180, 180, 180), 1));
  
  contourPlot_.attach(this);
  selectedStartingPointPlot_.attach(this);
  
  pickMode_ = IterationPlot::PICK_COORDINATES;
  
  plotPanner_ = new QwtPlotPanner(canvas());
  picker_     = new QwtPicker(canvas());
  plotZoomer_ = new QwtPlotZoomer(canvas());
  
  plotPanner_->setEnabled(false);
  picker_->setEnabled(true);
  plotZoomer_->setEnabled(false);
  
  picker_->setSelectionFlags(QwtPicker::PointSelection);
  
  connect(picker_,     SIGNAL(selected(const QwtPolygon &)),  this, SLOT(pickPlotItem(const QwtPolygon &)));
  connect(plotPanner_, SIGNAL(panned(int, int)),              this, SLOT(panPlot(int, int)));
  connect(plotZoomer_, SIGNAL(zoomed(const QwtDoubleRect &)), this, SLOT(zoomPlot()));
}

IterationPlot::~IterationPlot()
{
  for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin();
      pl != iterPathPlots_.end(); pl++)
    delete (*pl);
}

void IterationPlot::clearIterates()
{
  for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin();
      pl != iterPathPlots_.end(); pl++)
    delete (*pl);
  for(int i = 0; i < NUM_ITER_PATH_COLORS_; i++)
    freeColorIndices_[i] = true;
  iterPathPlots_.clear();
  replot();
}

void IterationPlot::clearSelections()
{
  for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin();
      pl != iterPathPlots_.end(); pl++)
    (*pl)->clearSelections();
  iterPathSelected_ = false;
}

void IterationPlot::deactivateConstraints()
{
  constraintPlot_.detach();
  replot();
}

bool IterationPlot::getHold() const
{
  return hold_;
}

int IterationPlot::getMaxNumPlots() const
{
  return NUM_ITER_PATH_COLORS_;
}

double IterationPlot::getX1() const
{
  return canvasMap(QwtPlot::xBottom).s1();
}

double IterationPlot::getX2() const
{
  return canvasMap(QwtPlot::xBottom).s2();
}

double IterationPlot::getY1() const
{
  return canvasMap(QwtPlot::yLeft).s1();
}

double IterationPlot::getY2() const
{
  return canvasMap(QwtPlot::yLeft).s2();
}

double IterationPlot::getZ1() const
{
  return contourData_.range().minValue();
}

double IterationPlot::getZ2() const
{
  return contourData_.range().maxValue();
}

void IterationPlot::panPlot(int dx, int dy)
{
  emit plotPanned(dx, dy);
}

void IterationPlot::pickPlotItem(const QwtPolygon &poly)
{
  QPoint p = poly.point(0);
  double xs = invTransform(QwtPlot::xBottom, p.x());
  double ys = invTransform(QwtPlot::yLeft, p.y());
  
  if(pickMode_ == IterationPlot::PICK_ITER_PATH)
  {
    int i = 0;
    for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin();
        pl != iterPathPlots_.end(); pl++)
    {
      if((*pl)->pick(p))
      {
        iterPathSelected_ = true;
        selectedIterPath_ = pl;
        
        pl++;
        for(std::list< PlotIterPath * >::iterator pl_ = pl;
            pl_ != iterPathPlots_.end(); pl_++)
          (*pl_)->clearSelections();
        
        //emit(selectedIterPathChanged(i));
        return;
      }
      
      (*pl)->clearSelections();
      i++;
    }
    
    iterPathSelected_ = false;
  }
  else if(pickMode_ == IterationPlot::PICK_ITERATE)
  {
    double x, y;
    for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin(); 
        pl != iterPathPlots_.end(); pl++)
    {
      if((*pl)->pickIterate(p, x, y))
      {
        pl++;
        for(std::list< PlotIterPath * >::iterator pl_ = pl; 
            pl_ != iterPathPlots_.end(); pl_++)
          (*pl_)->clearSelections();
        emit selectedIterateChanged(x, y);
        return;
      }
      
      (*pl)->clearSelections();
    }
  }
  else if(pickMode_ == IterationPlot::PICK_SEARCH_LINE)
  {
    double x1, y1, x2, y2;
    for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin(); 
        pl != iterPathPlots_.end(); pl++)
    {
      if((*pl)->pickSearchLine(p, x1, y1, x2, y2))
      {
        pl++;
        for(std::list< PlotIterPath * >::iterator pl_ = pl; 
            pl_ != iterPathPlots_.end(); pl_++)
          (*pl_)->clearSelections();
        emit(selectedLineChanged(x1, y1, x2, y2));
        return;
      }
      
      (*pl)->clearSelections();
    }
  }
  else if(pickMode_ == IterationPlot::PICK_COORDINATES)
  {
    selectedStartingPointPlot_.setData(&xs, &ys, 1);
    replot();
    emit coordinatesPicked(xs, ys);
  }
  else if(pickMode_ == IterationPlot::PICK_CONSTRAINTS)
  {
    double x[2], y[2];
    for(int i = 0; i < 2; i++)
    {
      x[i] = invTransform(QwtPlot::xBottom, poly.point(i).x());
      y[i] = invTransform(QwtPlot::yLeft, poly.point(i).y());
    }
    double x1 = std::min(x[0], x[1]);
    double x2 = std::max(x[0], x[1]);
    double y1 = std::min(y[0], y[1]);
    double y2 = std::max(y[0], y[1]);
    setConstraints(x1, x2, y1, y2);
    emit(constraintsPicked(x1, x2, y1, y2));
  }
}

void IterationPlot::zoomPlot()
{
  emit plotZoomed();
}

bool IterationPlot::insertIterationPath(const std::list< vector< double > > &iterPath,
                                        const std::list< matrix< double > > *simplices)
{
  int ci;
  bool found = false;
  
  for(ci = 0; ci < NUM_ITER_PATH_COLORS_; ci++)
  {
    if(freeColorIndices_[ci])
    {
      found = true;
      break;
    }
  }
  
  if(!found)
    return false;
  
  iterPathPlots_.push_back(new PlotIterPath(this));
  
  iterPathPlots_.back()->setIterationPath(iterPath);
  if(simplices != NULL)
    iterPathPlots_.back()->setSimplices(*simplices);
  
  freeColorIndices_[ci] = false;
    
  iterPathPlots_.back()->setColorPen(ITER_PATH_COLORS_[ci]);
  if(ci == 1)
    iterPathPlots_.front()->setColorPen(ITER_PATH_COLORS_[0]);
  
  replot();
  
  return true;
}

void IterationPlot::removeSelectedIterPath()
{
  if(iterPathSelected_)
  {
    int ci = 0;
    for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin(); 
        pl != selectedIterPath_; pl++)
      ci++;
    freeColorIndices_[ci] = true;
    
    delete (*selectedIterPath_);
    iterPathPlots_.erase(selectedIterPath_);
    iterPathSelected_ = false;
    
    replot();
  }
  else
    QMessageBox::critical(this, tr("Error"), tr("No iterates selected."));
}

void IterationPlot::selectPanTool()
{
  plotPanner_->setEnabled(true);
  picker_->setEnabled(false);
  plotZoomer_->setEnabled(false);
}

void IterationPlot::selectPickTool(PickMode pickMode)
{
  pickMode_ = pickMode;
  plotPanner_->setEnabled(false);
  picker_->setEnabled(true);
  plotZoomer_->setEnabled(false);
  
  if(pickMode == IterationPlot::PICK_CONSTRAINTS)
  {
    picker_->setSelectionFlags(QwtPicker::RectSelection);
    picker_->setRubberBand(QwtPicker::RectRubberBand);
  }
  else
  {
    picker_->setSelectionFlags(QwtPicker::PointSelection);
    picker_->setRubberBand(QwtPicker::NoRubberBand);
  }
}

void IterationPlot::selectZoomInTool()
{
  plotZoomer_->setZoomBase();
  
  plotPanner_->setEnabled(false);
  picker_->setEnabled(false);
  plotZoomer_->setEnabled(true);
}

void IterationPlot::setConstraints(double x1, double x2, double y1, double y2)
{
  double *px = new double[5];
  double *py = new double[5];
  
  px[0] = x1;
  py[0] = y1;
  px[1] = x2;
  py[1] = y1;
  px[2] = x2;
  py[2] = y2;
  px[3] = x1;
  py[3] = y2;
  px[4] = x1;
  py[4] = y1;
  
  constraintPlot_.setData(px, py, 5);
  constraintPlot_.attach(this);
  replot();
  
  delete[] px;
  delete[] py;
}

void IterationPlot::setFunction(Function *f)
{
  contourData_.setFunction(f);
  contourPlot_.setData(contourData_);
  contourPlot_.invalidateCache();
  replot();
}

void IterationPlot::setHold(bool hold)
{
  hold_ = hold;
}

void IterationPlot::setIterationPath(const std::list< vector< double > > &iterPath,
                                     const std::list< matrix< double > > *simplices)
{
  for(std::list< PlotIterPath * >::iterator pl = iterPathPlots_.begin();
      pl != iterPathPlots_.end(); pl++)
    delete (*pl);
  
  iterPathPlots_.clear();
  iterPathPlots_.push_back(new PlotIterPath(this));
  
  iterPathPlots_.back()->setIterationPath(iterPath);
  if(simplices != NULL)
    iterPathPlots_.back()->setSimplices(*simplices);
  
  iterPathPlots_.back()->setGrayPen();
  freeColorIndices_[0] = false;
  
  replot();
}

void IterationPlot::setPlotRange(double x1, double x2,
                                 double y1, double y2)
{
  QwtValueList contourLevels;
  
  setAxisScale(QwtPlot::xBottom, x1, x2);
  setAxisScale(QwtPlot::yLeft, y1, y2);
  
  replot();
}

void IterationPlot::setPlotRange(double x1, double x2,
                                 double y1, double y2,
                                 double z1, double z2)
{
  QwtValueList contourLevels;
  double levelArray[NUM_CONTOUR_LEVELS_];
  
  setAxisScale(QwtPlot::xBottom, x1, x2);
  setAxisScale(QwtPlot::yLeft, y1, y2);
  
  // TODO: allow the user to choose between linear and logarithmic z axes
  logSpace(NUM_CONTOUR_LEVELS_, z1, z2, levelArray);
  for(int li = 0; li < NUM_CONTOUR_LEVELS_; li++)
    contourLevels += levelArray[li];
  
  contourData_.setValueRange(z1, z2);
  contourPlot_.setData(contourData_);
  contourPlot_.setContourLevels(contourLevels);
  replot();
}
