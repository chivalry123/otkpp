
#include "surfacefunction.h"
#include "surfaceplot.h"

SurfacePlot::SurfacePlot(QWidget *parent) : Qwt3D::SurfacePlot(parent)
{
  function_ = new SurfaceFunction(this);
  function_->setMesh(50, 50);
  function_->create();
  
  initAxes(0, 0, 0, 0, 0, 0);
  
  setCoordinateStyle(Qwt3D::BOX);
  setZAxisScaleType(Qwt3D::LOG10SCALE);
  
  updateData();
  updateGL();
}

SurfacePlot::~SurfacePlot()
{
  delete function_;
}

void SurfacePlot::setFunction(const Function *f)
{
  SurfaceFunction_setFunction(f);
  function_->create();
  updateGL();
}

void SurfacePlot::setPlotRange(double x1, double x2, 
                               double y1, double y2, 
                               double z1, double z2)
{
  function_->setDomain(x1, x2, y1, y2);
  function_->setMinZ(z1);
  function_->setMaxZ(z2);
  function_->create();
  
  initAxes(x1, x2, y1, y2, z1, z2);
  
  updateGL();
}

void SurfacePlot::setZAxisScaleType(Qwt3D::SCALETYPE scaleType)
{
  function_->setZAxisScaleType(scaleType);
  function_->create();
  //initAxes(x1, x2, y1, y2, z1, z2);
  updateGL();
}

void SurfacePlot::initAxes(double x1, double x2, 
                           double y1, double y2, 
                           double z1, double z2)
{
  for(int i = 0; i < coordinates()->axes.size(); i++)
  {
    coordinates()->axes[i].setMajors(7);
    coordinates()->axes[i].setMinors(4);
  }
  
  coordinates()->axes[Qwt3D::X1].setLabelString("x");
  coordinates()->axes[Qwt3D::Y1].setLabelString("y");
  coordinates()->axes[Qwt3D::Z1].setLabelString("f(x,y)");
  
  coordinates()->axes[Qwt3D::X1].setLimits(x1, x2);
  coordinates()->axes[Qwt3D::X2].setLimits(x1, x2);
  coordinates()->axes[Qwt3D::X3].setLimits(x1, x2);
  coordinates()->axes[Qwt3D::X4].setLimits(x1, x2);
  coordinates()->axes[Qwt3D::Y1].setLimits(y1, y2);
  coordinates()->axes[Qwt3D::Y2].setLimits(y1, y2);
  coordinates()->axes[Qwt3D::Y3].setLimits(y1, y2);
  coordinates()->axes[Qwt3D::Y4].setLimits(y1, y2);
  coordinates()->axes[Qwt3D::Z1].setLimits(z1, z2);
  coordinates()->axes[Qwt3D::Z2].setLimits(z1, z2);
  coordinates()->axes[Qwt3D::Z3].setLimits(z1, z2);
  coordinates()->axes[Qwt3D::Z4].setLimits(z1, z2);
}
