
#ifndef OTKGUIAPPLICATION_H

#include "otkgui.h"
#include "otkguimodel.h"

#include <Function.h>

#include <QApplication>
#include <string>

class OTKGuiApplication : public QApplication
{
 public:
  OTKGuiApplication(int &argc, char **argv);
  ~OTKGuiApplication();
  
  //OTKGuiModel &getModel();
  
  //const OTKGuiModel &getModel() const;
 private:
  OTKGui *mainWindow_;
  OTKGuiModel *guiModel_;
};

#define OTKGUIAPPLICATION_H

#endif
