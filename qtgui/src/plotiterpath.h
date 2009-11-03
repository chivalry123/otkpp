
#ifndef PLOTITERPATH_H

#include "plotsimplexmesh.h"

#include <boost/numeric/ublas/vector.hpp>
#include <list>
#include <qwt_plot_curve.h>

using namespace boost::numeric::ublas;

class QLine;
class QPoint;
class QwtPlot;

class PlotIterPath
{
 public:
  PlotIterPath(QwtPlot *parent);
  ~PlotIterPath();
  
  void clearSelections();
  bool pick(const QPoint &p);
  bool pickIterate(const QPoint &p, double &x, double &y);
  bool pickSearchLine(const QPoint &p, double &x1, double &y1, double &x2, double &y2);
  void setColorPen(const QColor &color);
  void setGrayPen();
  void setIterationPath(const std::list< vector< double > > &iterPath);
  void setSimplices(const std::list< matrix< double > > &simplices);
 private:
  bool isSelected_;
  std::list< vector< double > > iterPath_;
  QwtPlotCurve iterPathPlot_;
  QwtPlot *parent_;
  QwtPlotCurve selectedIteratePlot_;
  QwtPlotCurve selectedLinePlot_;
  PlotSimplexMesh simplexPlot_;
  
  PlotIterPath() { }
  bool findIterate_(const QPoint &p, double &x, double &y);
  bool findSearchLine_(const QPoint &p, double &x1, double &y1, double &x2, double &y2);
  double pointDistToLine_(const QPoint &p, const QLine &l);
};

#define PLOTITERPATH_H

#endif
