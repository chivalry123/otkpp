
#ifndef LOGARITHMICCOLORMAP_H

#include <qwt_color_map.h>

class LogarithmicColorMap : public QwtColorMap
{
public:
  //LogarithmicColorMap();
  QwtColorMap *copy() const;
  QRgb rgb(const QwtDoubleInterval &interval, double value) const;
  unsigned char colorIndex (const QwtDoubleInterval &interval, double value) const;
};

#define LOGARITHMICCOLORMAP_H

#endif
