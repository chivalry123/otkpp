
#ifndef SURFACEFUNCTION_H

#include <qwt3d_function.h>

class Function;

class SurfaceFunction : public Qwt3D::Function
{
public:
  SurfaceFunction(Qwt3D::SurfacePlot *pw);
  
  double operator()(double x, double y);
  
  void setDomain(double minu, double maxu, double minv, double maxv);
  
  void setMinZ(double val);
  
  void setMaxZ(double val);
  
  void setZAxisScaleType(Qwt3D::SCALETYPE scaleType);
private:
  //Function *f_;
  double xMin_;
  double xMax_;
  double yMin_;
  double yMax_;
  double zMin_;
  double zMax_;
  Qwt3D::SCALETYPE zScaleType_;
};

void SurfaceFunction_setFunction(const Function *f); // TODO: move this out of here!

#define SURFACEFUNCTION_H

#endif
