
SET(INST_HEADERS "AbstractGradientSolver.h"
                 "ExternalSolver.h"
                 "Solver.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/localsolvers)

ADD_SUBDIRECTORY(gslsolvers)
ADD_SUBDIRECTORY(lbfgsb)
ADD_SUBDIRECTORY(lmbm)
ADD_SUBDIRECTORY(native)
