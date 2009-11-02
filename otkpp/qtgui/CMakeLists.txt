
FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${OTKPP_INCLUDE_DIRS}
                    ${QT_INCLUDE_DIRS}
                    ${QWT_INCLUDE_DIR}
                    ${QWTPLOT3D_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(OTKGUI_HEADERS "src/contourdata.h"
                   "src/iterationplot.h"
                   "src/otkgui.h")

SET(OTKGUI_SOURCES "src/contourdata.cpp"
                   "src/iterationplot.cpp"
                   "src/logarithmiccolormap.cpp"
                   "src/main.cpp"
                   "src/minimizerthread.cpp"
                   "src/otkguiapplication.cpp"
                   "src/otkgui.cpp"
                   "src/otkguimodel.cpp"
                   "src/plotiterpath.cpp"
                   "src/plotsimplexmesh.cpp"
                   "src/searchlineplot.cpp"
                   "src/surfacefunction.cpp"
                   "src/surfaceplot.cpp")

QT4_WRAP_UI(OTKGUI_UI_HEADERS src/mainwindow.ui)
QT4_WRAP_CPP(OTKGUI_MOC_SRCS ${OTKGUI_HEADERS})

include_directories(${INCLUDE_DIRECTORIES})

ADD_EXECUTABLE(otkgui ${OTKGUI_SOURCES} ${OTKGUI_UI_HEADERS} ${OTKGUI_MOC_SRCS})
TARGET_LINK_LIBRARIES(otkgui otkpp qwt-qt4 qwtplot3d-qt4)

IF(WITH_FORTRAN)
  TARGET_LINK_LIBRARIES(otkgui otkpp_fortran gfortran)
ENDIF(WITH_FORTRAN)

IF(WITH_GSL)
  TARGET_LINK_LIBRARIES(otkgui gsl gslcblas)
ENDIF(WITH_GSL)

IF(WITH_LIBMATHEVAL)
  TARGET_LINK_LIBRARIES(otkgui matheval)
ENDIF(WITH_LIBMATHEVAL)

INSTALL(TARGETS otkgui DESTINATION bin)
