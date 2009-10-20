
#include "plotiterpath.h"

#include <qwt_plot.h>
#include <qwt_symbol.h>

PlotIterPath::PlotIterPath(QwtPlot *parent) : parent_(parent)
{
  QwtSymbol sym;
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setBrush(QBrush(Qt::gray));
  sym.setPen(QPen(Qt::black, 1));
  sym.setSize(10);
  
  iterPathPlot_.setSymbol(sym);
  iterPathPlot_.setPen(QPen(Qt::black, 3));
  
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setBrush(QBrush(Qt::red));
  sym.setPen(QPen(Qt::red, 1));
  sym.setSize(10);
  selectedIteratePlot_.setSymbol(sym);
  
  selectedLinePlot_.setPen(QPen(Qt::red, 3));
  
  iterPathPlot_.attach(parent_);
  selectedIteratePlot_.attach(parent_);
  selectedLinePlot_.attach(parent_);
}

PlotIterPath::~PlotIterPath()
{
  iterPathPlot_.detach();
  selectedIteratePlot_.detach();
  selectedLinePlot_.detach();
  simplexPlot_.detach();
}

void PlotIterPath::clearSelections()
{
  selectedIteratePlot_.detach();
  selectedLinePlot_.detach();
  iterPathPlot_.draw(0, iterPathPlot_.dataSize() - 1);
}

bool PlotIterPath::pick(const QPoint &p)
{
  double x1, y1, x2, y2;
  
  if(findSearchLine_(p, x1, y1, x2, y2))
  {
    QColor oldColor = iterPathPlot_.pen().color();
    setColorPen(Qt::red);
    iterPathPlot_.draw(0, iterPathPlot_.dataSize() - 1);
    if(oldColor != Qt::black)
      setColorPen(oldColor);
    else
      setGrayPen();
    return true;
  }
  
  iterPathPlot_.draw(0, iterPathPlot_.dataSize() - 1);
  return false;
}

bool PlotIterPath::pickIterate(const QPoint &p, double &x, double &y)
{
  if(findIterate_(p, x, y))
  {
    selectedIteratePlot_.setData(&x, &y, 1);
    selectedIteratePlot_.attach(parent_);
    selectedLinePlot_.detach();
    iterPathPlot_.draw(0, iterPathPlot_.dataSize() - 1);
    selectedIteratePlot_.draw(0, 0);
    
    return true;
  }
  
  return false;
}

bool PlotIterPath::pickSearchLine(const QPoint &p, double &x1, double &y1, double &x2, double &y2)
{
  if(findSearchLine_(p, x1, y1, x2, y2))
  {
    double lx[] = { x1, x2 };
    double ly[] = { y1, y2 };
    
    selectedLinePlot_.setData(lx, ly, 2);
    selectedLinePlot_.attach(parent_);
    selectedIteratePlot_.detach();
    iterPathPlot_.draw(0, iterPathPlot_.dataSize() - 1);
    selectedLinePlot_.draw(0, 1);
    
    return true;
  }
  
  return false;
}

void PlotIterPath::setColorPen(const QColor &color)
{
  QwtSymbol sym;
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setBrush(QBrush(color));
  sym.setPen(QPen(color, 1));
  sym.setSize(10);
  
  iterPathPlot_.setSymbol(sym);
  iterPathPlot_.setPen(QPen(color, 3));
  
  sym.setBrush(QBrush(Qt::red));
  sym.setPen(QPen(Qt::red, 1));
  sym.setSize(10);
  selectedIteratePlot_.setSymbol(sym);
  
  selectedLinePlot_.setPen(QPen(Qt::red, 3));
}

void PlotIterPath::setGrayPen()
{
  QwtSymbol sym;
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setBrush(QBrush(Qt::gray));
  sym.setPen(QPen(Qt::black, 1));
  sym.setSize(10);
  
  iterPathPlot_.setSymbol(sym);
  iterPathPlot_.setPen(QPen(Qt::black, 3));
  
  sym.setBrush(QBrush(Qt::red));
  sym.setPen(QPen(Qt::red, 1));
  sym.setSize(10);
  selectedIteratePlot_.setSymbol(sym);
  
  selectedLinePlot_.setPen(QPen(Qt::red, 3));
}

void PlotIterPath::setIterationPath(const std::list< vector< double > > &iterPath)
{
  const int N = iterPath.size();
  double *px = new double[N];
  double *py = new double[N];
  std::list< vector< double > >::const_iterator it;
  
  int idx = 0;
  for(it = iterPath.begin(); it != iterPath.end(); it++)
  {
    px[idx] = (*it)(0);
    py[idx] = (*it)(1);
    idx++;
  }
  
  iterPathPlot_.setData(px, py, N);
  iterPath_ = iterPath;
  
  simplexPlot_.detach();
  
  delete[] px;
  delete[] py;
}

void PlotIterPath::setSimplices(const std::list< matrix< double > > &simplices)
{
  simplexPlot_.setSimplexData(simplices);
  simplexPlot_.attach(parent_);
}

bool PlotIterPath::findIterate_(const QPoint &p, double &x, double &y)
{
  double dist;
  double dx, dy;
  double nearestDist = 1e10;
  int nearestIndex = -1;
  std::list< vector < double > >::const_iterator pIter;
  std::list< vector < double > >::const_iterator spIter;
  double xc, yc;
  
  if(iterPath_.size() == 0)
    return false;
  
  int idx = 0;
  for(pIter = iterPath_.begin(); pIter != iterPath_.end(); pIter++)
  {
    xc = parent_->transform(QwtPlot::xBottom, (*pIter)(0));
    yc = parent_->transform(QwtPlot::yLeft, (*pIter)(1));
    dx = xc - p.x();
    dy = yc - p.y();
    dist = sqrt(dx*dx + dy*dy);
    if(dist < 5 && dist < nearestDist)
    {
      nearestDist = dist;
      nearestIndex = idx;
      spIter = pIter;
    }
    idx++;
  }
  
  if(nearestIndex != -1)
  {
    x = (*spIter)(0);
    y = (*spIter)(1);
    
    return true;
  }
  
  return false;
}

bool PlotIterPath::findSearchLine_(const QPoint &p, double &x1, double &y1, double &x2, double &y2)
{
  QLine l;
  QPoint p1, p2;
  double pDist;
  std::list< vector < double > >::const_iterator pIter;
  std::list< vector < double > >::const_iterator prevPIter = iterPath_.end();
  double xc1, yc1, xc2, yc2;
  
  if(iterPath_.size() == 0)
    return false;
  
  for(pIter = iterPath_.begin(); pIter != iterPath_.end(); pIter++)
  {
    if(prevPIter != iterPath_.end())
    {
      xc1 = parent_->transform(QwtPlot::xBottom, (*prevPIter)(0));
      yc1 = parent_->transform(QwtPlot::yLeft,   (*prevPIter)(1));
      xc2 = parent_->transform(QwtPlot::xBottom, (*pIter)(0));
      yc2 = parent_->transform(QwtPlot::yLeft,   (*pIter)(1));
      l = QLine(QPoint(xc1, yc1), QPoint(xc2, yc2));
      pDist = pointDistToLine_(p, l);
      if(pDist < 5.0)
      {
        x1 = (*prevPIter)(0);
        y1 = (*prevPIter)(1);
        x2 = (*pIter)(0);
        y2 = (*pIter)(1);
        
        return true;
      }
    }
    
    prevPIter = pIter;
  }
  
  return false;
}

double PlotIterPath::pointDistToLine_(const QPoint &p, const QLine &l)
{
  // Project the point on the line.
  double lx = l.x2() - l.x1();
  double ly = l.y2() - l.y1();
  double ll = sqrt(lx*lx + ly*ly);
  
  double vx = p.x() - l.x1();
  double vy = p.y() - l.y1();
  
  double prPar = (vx*lx + vy*ly) / ll;
  double prPerp = (vx*ly - vy*lx) / ll;
  
  // The point is on the "left" side of the line.
  if(prPar < 0)
    return sqrt(vx*vx + vy*vy);
  // The point is on the "right" side of the line.
  else if(prPar > ll)
    return sqrt(vx*vx + vy*vy);
  else
    return fabs(prPerp);
}
