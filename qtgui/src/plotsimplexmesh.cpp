
#include "plotsimplexmesh.h"

#include <QPainter>
#include <qwt_scale_map.h>

void PlotSimplexMesh::draw(QPainter *painter,
                           const QwtScaleMap &xMap,
                           const QwtScaleMap &yMap,
                           const QRect &canvasRect) const
{
  QPoint p1, p2;
  std::list< matrix< double > >::const_iterator s;
  double x1, x2, y1, y2;
  
  painter->setPen(QPen(Qt::black, 0.5));
  
  for(s = simplices_.begin(); s != simplices_.end(); s++)
  {
    for(int i = 0; i < 3; i++)
    {
      x1 = xMap.transform((*s)(0, i));
      y1 = yMap.transform((*s)(1, i));
      x2 = xMap.transform((*s)(0, (i+1) % 3));
      y2 = yMap.transform((*s)(1, (i+1) % 3));
      
      p1 = QPoint(x1, y1);
      p2 = QPoint(x2, y2);
      painter->drawLine(QLine(p1, p2));
    }
  }
}

int PlotSimplexMesh::rtti() const
{
  return QwtPlotItem::Rtti_PlotUserItem;
}

void PlotSimplexMesh::setSimplexData(const std::list< matrix< double > > &simplices)
{
  simplices_ = simplices;
}
