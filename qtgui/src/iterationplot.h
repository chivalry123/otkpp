
#ifndef ITERATIONPLOT_H

#include "contourdata.h"
#include "plotiterpath.h"

#include <qwt_picker.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>

using namespace boost::numeric::ublas;

class IterationPlot : public QwtPlot
{
  Q_OBJECT
 public:
  enum PickMode
  {
    PICK_COORDINATES,
    PICK_CONSTRAINTS,
    PICK_ITER_PATH,
    PICK_ITERATE,
    PICK_SEARCH_LINE
  };
  
  IterationPlot(QWidget *parent);
  ~IterationPlot();
  
  void clearIterates();
  void clearSelections();
  void deactivateConstraints();
  bool getHold() const;
  int getMaxNumPlots() const;
  double getX1() const;
  double getX2() const;
  double getY1() const;
  double getY2() const;
  double getZ1() const;
  double getZ2() const;
  bool insertIterationPath(const std::list< vector< double > > &iterPath,
                           const std::list< matrix< double > > *simplices = NULL);
  void removeSelectedIterPath();
  void selectPanTool();
  void selectPickTool(PickMode pickMode);
  void selectZoomInTool();
  void setConstraints(double x1, double x2, double y1, double y2);
  void setFunction(Function *f);
  void setHold(bool hold);
  void setIterationPath(const std::list< vector< double > > &iterPath,
                        const std::list< matrix< double > > *simplices = NULL);
  void setPlotRange(double x1, double x2,
                    double y1, double y2);
  void setPlotRange(double x1, double x2,
                    double y1, double y2,
                    double z1, double z2);
 signals:
  void constraintsPicked(double, double, double, double);
  void coordinatesPicked(double, double);
  void plotPanned(int, int);
  void plotZoomed();
  void selectedIterateChanged(double, double);
  //void selectedIterPathChanged(int);
  void selectedLineChanged(double, double, double, double);
 private slots:
  void panPlot(int dx, int dy);
  void pickPlotItem(const QwtPolygon &poly);
  void zoomPlot(/*const QwtDoubleRect &rect*/);
 private:
  static const QColor ITER_PATH_COLORS_[];
  static const int NUM_CONTOUR_LEVELS_;
  static const int NUM_ITER_PATH_COLORS_ = 5;
  
  QwtPlotCurve constraintPlot_;
  ContourData contourData_;
  QwtPlotSpectrogram contourPlot_;
  bool freeColorIndices_[NUM_ITER_PATH_COLORS_];
  bool hold_;
  std::list< PlotIterPath * > iterPathPlots_;
  bool iterPathSelected_;
  PickMode pickMode_;
  QwtPicker *picker_;
  QwtPlotPanner *plotPanner_;
  QwtPlotZoomer *plotZoomer_;
  std::list< PlotIterPath * >::iterator selectedIterPath_;
  QwtPlotCurve selectedStartingPointPlot_;
};

#define ITERATIONPLOT_H

#endif
