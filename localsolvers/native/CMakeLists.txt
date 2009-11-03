
SET(INST_HEADERS "ConjGradMT.h"
                 "DoglegBFGS.h"
                 "GradientSolver.h"
                 "HookeJeeves.h"
                 "LinminBFGS.h"
                 "MNewton.h"
                 "NativeSolver.h"
                 "PARTAN.h"
                 "SteihaugSR1.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/localsolvers/native)

ADD_SUBDIRECTORY(linmin)
ADD_SUBDIRECTORY(trsreg)
