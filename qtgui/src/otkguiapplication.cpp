
#include "otkguiapplication.h"

#include <iostream>

OTKGuiApplication::OTKGuiApplication(int &argc, char **argv) : 
  QApplication(argc, argv)
{
  guiModel_ = new OTKGuiModel();
  
  //Q_INIT_RESOURCE(this);
  mainWindow_ = new OTKGui(this, guiModel_);
  mainWindow_->show();
  
  /*connect(model_, SIGNAL(objectiveFunctionChanged(Function *)),
          mainWindow_->getWidgets()->iterationPlot,
          SLOT(setFunction(Function *)));*/
}

OTKGuiApplication::~OTKGuiApplication()
{
  delete mainWindow_;
  delete guiModel_;
}

/*OTKGuiModel &OTKGuiApplication::getModel()
{
  return *model_;
}

const OTKGuiModel &OTKGuiApplication::getModel() const
{
  return *model_;
}*/
