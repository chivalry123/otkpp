
#include "searchlineplot.h"

#include <Function.h>

#include <qwt_symbol.h>

const int SearchLinePlot::NUM_PLOT_POINTS_ = 50;

SearchLinePlot::SearchLinePlot(QWidget *parent) : QwtPlot(parent)
{
  setAxisScale(QwtPlot::xBottom, 0.0, 2.0);
  axisAutoScale(QwtPlot::yLeft);
  
  QwtSymbol sym;
  sym.setStyle(QwtSymbol::Ellipse);
  sym.setPen(QPen(Qt::black, 1));
  sym.setSize(10);
  //iterPathPlot_.setSymbol(sym);
  //iterPathPlot_.setPen(QPen(Qt::black, 3));
  
  //stepPoint_.setPen(sym);
  stepPoint_.setSymbol(sym);
  
  graph_.attach(this);
  stepPoint_.attach(this);
}

void SearchLinePlot::setFunction(Function *f)
{
  f_ = f;
}

void SearchLinePlot::setSearchLine(double x1, double y1,
                                   double x2, double y2)
{
  double fxOne;
  double one;
  vector< double > p1(2);
  vector< double > p2(2);
  
  p1(0) = x1;
  p1(1) = y1;
  p2(0) = x2;
  p2(1) = y2;
  
  lineOrigin_ = p1;
  lineSegment_ = p2 - p1;
  
  double x[NUM_PLOT_POINTS_];
  double fx[NUM_PLOT_POINTS_];
  
  for(int i = 0; i < NUM_PLOT_POINTS_; i++)
  {
    x[i] = i / (NUM_PLOT_POINTS_ - 1.0) * 2.0;
    fx[i] = phi_(x[i]);
  }
  
  graph_.setData(x, fx, NUM_PLOT_POINTS_);
  one = 1.0;
  fxOne = phi_(1.0);
  stepPoint_.setData(&one, &fxOne, 1);
  replot();
}

double SearchLinePlot::phi_(double alpha)
{
  vector< double > x = lineOrigin_;
  x = x + alpha * lineSegment_;
  return (*f_)(x);
}
