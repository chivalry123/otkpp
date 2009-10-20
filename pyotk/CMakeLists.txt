
IF(WITH_PYTHON)
  INCLUDE_DIRECTORIES(${OTKPP_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})
  
  SET(PYOTK_DRIVER_SRCS "../python/driver.cpp")
  SET(PYOTK_PLOTUTILS_SRCS "../python/plotutils.cpp")
  
  ADD_LIBRARY(pyotk_driver SHARED ${PYOTK_DRIVER_SRCS})
  TARGET_LINK_LIBRARIES(pyotk_driver otkpp boost_python matheval rt)
  IF(WITH_FORTRAN)
  TARGET_LINK_LIBRARIES(pyotk_driver gfortran otkpp_fortran)
  ENDIF(WITH_FORTRAN)
  IF(WITH_GSL)
  TARGET_LINK_LIBRARIES(pyotk_driver gsl gslcblas)
  ENDIF(WITH_GSL)
  
  ADD_LIBRARY(pyotk_plotutils SHARED ${PYOTK_PLOTUTILS_SRCS})
  TARGET_LINK_LIBRARIES(pyotk_plotutils otkpp boost_python)
  #INSTALL(TARGETS pyotk_driver LIBRARY DESTINATION lib)
ENDIF(WITH_PYTHON)
