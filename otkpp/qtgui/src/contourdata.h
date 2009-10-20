
#ifndef CONTOURDATA_H

#include <qwt_raster_data.h>

class Function;

class ContourData : public QwtRasterData
{
public:
  ContourData();
  
  ContourData *copy() const;
  
  QwtDoubleInterval range() const;
  
  double value(double x, double y) const;
  
  void setFunction(Function *f);
  
  void setValueRange(double valMin, double valMax);
private:
  Function *f_;
  double valMin_;
  double valMax_;
};

#define CONTOURDATA_H

#endif
