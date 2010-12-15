PROJECT(otkpp CXX C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)
#ADD_DEFINITIONS(-Wall -O2 -DNDEBUG)

OPTION(WITH_FORTRAN "compile with additional algorithms implemented in Fortran" ON)
OPTION(WITH_GSL "compile with additional GSL algorithms" ON)
OPTION(WITH_LIBMATHEVAL "compile with symbolic evaluation support" ON)
OPTION(WITH_QT "compile with Qt-based gui" ON)

SET(OTKPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../"
                       ${CMAKE_CURRENT_SOURCE_DIR}
                       "${CMAKE_CURRENT_SOURCE_DIR}/constraints"
                       "${CMAKE_CURRENT_SOURCE_DIR}/objfunc"
                       "${CMAKE_CURRENT_SOURCE_DIR}/interpolation"
                       "${CMAKE_CURRENT_SOURCE_DIR}/lib"
                       "${CMAKE_CURRENT_SOURCE_DIR}/linalg"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers/gslsolvers"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers/lbfgsb"
                       #"${CMAKE_CURRENT_SOURCE_DIR}/solvers/lmbm"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers/native"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers/native/linmin"
                       "${CMAKE_CURRENT_SOURCE_DIR}/solvers/native/trsreg"
                       #"${CMAKE_CURRENT_SOURCE_DIR}/solvers/newuoa"
                       "${CMAKE_CURRENT_SOURCE_DIR}/stopcrit"
                       "${CMAKE_CURRENT_SOURCE_DIR}/testproblems"
                       "${CMAKE_CURRENT_SOURCE_DIR}/tests")

IF(NOT QWT_INCLUDE_DIR)
  SET(QWT_INCLUDE_DIR "/usr/include/qwt-qt4")
ENDIF(NOT QWT_INCLUDE_DIR)
IF(NOT QWTPLOT3D_INCLUDE_DIR)
  SET(QWTPLOT3D_INCLUDE_DIR "/usr/include/qwtplot3d-qt4")
ENDIF(NOT QWTPLOT3D_INCLUDE_DIR)

FIND_PACKAGE(Boost REQUIRED)

IF(WITH_FORTRAN)
  ENABLE_LANGUAGE(Fortran)
  ADD_DEFINITIONS(-DWITH_FORTRAN)
ENDIF(WITH_FORTRAN)

IF(WITH_GSL)
  ADD_DEFINITIONS(-DWITH_GSL)
ENDIF(WITH_GSL)

IF(WITH_LIBMATHEVAL)
  ADD_DEFINITIONS(-DWITH_LIBMATHEVAL)
ENDIF(WITH_LIBMATHEVAL)

IF(WITH_QT)
  FIND_PACKAGE(Qt4)
ENDIF(WITH_QT)

ADD_SUBDIRECTORY(constraints)
ADD_SUBDIRECTORY(objfunc)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(linalg)
ADD_SUBDIRECTORY(solvers)
IF(WITH_QT)
  ADD_SUBDIRECTORY(qtgui)
ENDIF(WITH_QT)
ADD_SUBDIRECTORY(stopcrit)
ADD_SUBDIRECTORY(testproblems)
ADD_SUBDIRECTORY(tests)

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

INSTALL(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/otkpp)
