
#ifndef SURFACEPLOT_H

#include <qwt3d_surfaceplot.h>
#include <qwt3d_function.h>

class Function;
class SurfaceFunction;

class SurfacePlot : public Qwt3D::SurfacePlot
{
public:
  SurfacePlot(QWidget *parent);
  
  ~SurfacePlot();
  
  void setFunction(const Function *f);
  
  void setPlotRange(double x1, double x2, 
                    double y1, double y2, 
                    double z1, double z2);
  
  void setZAxisScaleType(Qwt3D::SCALETYPE scaleType);
private:
  SurfaceFunction *function_;
  
  void initAxes(double x1, double x2, 
                double y1, double y2, 
                double z1, double z2);
};

#define SURFACEPLOT_H

#endif
