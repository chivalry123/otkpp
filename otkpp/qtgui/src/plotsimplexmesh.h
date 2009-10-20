
#ifndef PLOTSIMPLEXMESH_H

#include <boost/numeric/ublas/matrix.hpp>
#include <qwt_plot_item.h>

using namespace boost::numeric::ublas;

class PlotSimplexMesh : public QwtPlotItem
{
 public:
  void draw(QPainter *painter,
            const QwtScaleMap &xMap,
            const QwtScaleMap &yMap,
            const QRect &canvasRect) const;
  virtual int rtti() const;
  void setSimplexData(const std::list< matrix< double > > &simplices);
 private:
  std::list< matrix< double > > simplices_;
};

#define PLOTSIMPLEXMESH_H

#endif
