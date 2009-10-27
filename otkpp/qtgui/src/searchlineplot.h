
#ifndef SEARCHLINEPLOT_H

#include <boost/numeric/ublas/vector.hpp>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

using namespace boost::numeric::ublas;

class Function;

class SearchLinePlot : public QwtPlot
{
 public:
  SearchLinePlot(QWidget *parent);
  void setFunction(Function *f);
  void setSearchLine(double x1, double y1,
                     double x2, double y2);
 private:
  static const int NUM_PLOT_POINTS_;
  
  Function *f_;
  QwtPlotCurve graph_;
  double lineLength_;
  vector< double > lineSegment_;
  vector< double > lineOrigin_;
  QwtPlotCurve stepPoint_;
  
  double phi_(double alpha);
};

#define SEARCHLINEPLOT_H

#endif
