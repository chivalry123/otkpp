
INCLUDE_DIRECTORIES(${OTKPP_INCLUDE_DIRS})

ADD_EXECUTABLE(solvertests solvertests.cpp)

TARGET_LINK_LIBRARIES(solvertests otkpp rt boost_unit_test_framework)

IF(WITH_FORTRAN)
  TARGET_LINK_LIBRARIES(solvertests otkpp_fortran)
ENDIF(WITH_FORTRAN)

IF(WITH_GSL)
  TARGET_LINK_LIBRARIES(solvertests gsl gslcblas)
ENDIF(WITH_GSL)

IF(WITH_LIBMATHEVAL)
  TARGET_LINK_LIBRARIES(solvertests matheval)
ENDIF(WITH_LIBMATHEVAL)

ENABLE_TESTING()
ADD_TEST(solvertests solvertests)

#INSTALL(TARGETS otktest DESTINATION bin)
