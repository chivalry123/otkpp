PROJECT(otkpp CXX Fortran C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -DNDEBUG)

SET(OTKPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                       "${CMAKE_CURRENT_SOURCE_DIR}/constraints"
                       "${CMAKE_CURRENT_SOURCE_DIR}/function"
                       "${CMAKE_CURRENT_SOURCE_DIR}/lib"
                       "${CMAKE_CURRENT_SOURCE_DIR}/linalg"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/gslsolvers"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/lbfgsb"
                       #"${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/lmbm"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/native"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/native/linmin"
                       "${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/native/trsreg"
                       #"${CMAKE_CURRENT_SOURCE_DIR}/localsolvers/newuoa"
                       "${CMAKE_CURRENT_SOURCE_DIR}/stopcrit"
                       "${CMAKE_CURRENT_SOURCE_DIR}/testproblems"
                       "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#IF(NOT PYTHON_INCLUDE_DIR)
#  SET(PYTHON_INCLUDE_DIR "/usr/include/python2.5")
#ENDIF(NOT PYTHON_INCLUDE_DIR)
IF(NOT QWT_INCLUDE_DIR)
  SET(QWT_INCLUDE_DIR "/usr/include/qwt-qt4")
ENDIF(NOT QWT_INCLUDE_DIR)
IF(NOT QWTPLOT3D_INCLUDE_DIR)
  SET(QWTPLOT3D_INCLUDE_DIR "/usr/include/qwtplot3d-qt4")
ENDIF(NOT QWTPLOT3D_INCLUDE_DIR)

OPTION(WITH_FORTRAN "compile additional algorithms implemented in Fortran" ON)
OPTION(WITH_GSL "compile with additional GSL algorithms" ON)
OPTION(WITH_LIBMATHEVAL "compile with symbolic evaluation support" ON)
#OPTION(WITH_PYTHON "compile with Python support" ON)
OPTION(WITH_QT "compile with Qt-based gui" ON)
SET(QT_UIC_EXECUTABLE "" uic-qt4)
SET(QT_MOC_EXECUTABLE "" moc-qt4)

IF(WITH_FORTRAN)
  ADD_DEFINITIONS(-DWITH_FORTRAN)
ENDIF(WITH_FORTRAN)

IF(WITH_GSL)
  ADD_DEFINITIONS(-DWITH_GSL)
ENDIF(WITH_GSL)

IF(WITH_LIBMATHEVAL)
  ADD_DEFINITIONS(-DWITH_LIBMATHEVAL)
ENDIF(WITH_LIBMATHEVAL)

ADD_SUBDIRECTORY(constraints)
ADD_SUBDIRECTORY(function)
#ADD_SUBDIRECTORY(globalsolvers)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(linalg)
ADD_SUBDIRECTORY(localsolvers)
#IF(WITH_PYTHON)
#  ADD_SUBDIRECTORY(python)
#ENDIF(WITH_PYTHON)
IF(WITH_QT)
  ADD_SUBDIRECTORY(qtgui)
ENDIF(WITH_QT)
ADD_SUBDIRECTORY(stopcrit)
ADD_SUBDIRECTORY(testproblems)
ADD_SUBDIRECTORY(tests)

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/config.h DESTINATION include/otkpp)
