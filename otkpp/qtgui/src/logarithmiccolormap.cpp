
#include "logarithmiccolormap.h"

#include <math.h>

/*LogarithmicColorMap::LogarithmicColorMap() : QwtColorMap(QwtColorMap::Indexed)
{ }*/

QwtColorMap *LogarithmicColorMap::copy() const
{
  return new LogarithmicColorMap(*this);
}

QRgb LogarithmicColorMap::rgb(const QwtDoubleInterval &interval, double value) const
{
  double a = interval.minValue();
  double b = interval.maxValue();
  //double i = (exp(log(a) + (value-a) / (b-a) * (log(b)-log(a))) - a) / (b-a) * 255.0;
  double i = (log(exp(a) + (value-a) / (b-a) * (exp(b)-exp(a))) - a) / (b-a) * 255.0;
  if(i >= 0 && i <= 255)
    return QColor(i, i, i).rgb();
  else if(i <= 0)
    return QColor(0, 0, 0).rgb();
  else
    return QColor(255, 255, 255).rgb();
}

unsigned char LogarithmicColorMap::colorIndex(const QwtDoubleInterval &interval, double value) const
{
  double a = interval.minValue();
  double b = interval.maxValue();
  //double i = exp(log(a) + (value-a) / (b-a) * (log(b)-log(a)) - a) / (b-a) * 255.0;
  double i = (log(exp(a) + (value-a) / (b-a) * (exp(b)-exp(a))) - a) / (b-a) * 255.0;
  if(i >= 0 && i <= 255)
    return i;
  else if(i <= 0)
    return 0;
  else
    return 255;
}
