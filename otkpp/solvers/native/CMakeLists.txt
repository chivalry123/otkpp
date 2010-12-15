
SET(INST_HEADERS "ConjGradMT.h"
                 "DoglegBFGS.h"
                 "DSQA.h"
                 "HookeJeeves.h"
                 "LinminBFGS.h"
                 "LRWWSimplex.h"
                 "MNewton.h"
                 "NativeGradientSolver.h"
                 "NativeSolver.h"
                 "PARTAN.h"
                 "Rosenbrock.h"
                 "SteihaugSR1.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/localsolvers/native)

ADD_SUBDIRECTORY(linmin)
ADD_SUBDIRECTORY(trsreg)

