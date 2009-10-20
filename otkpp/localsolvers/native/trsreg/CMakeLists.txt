
SET(INST_HEADERS "DoglegSolver.h"
                 "SteihaugSolver.h"
                 "TrustRegionSolver.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/localsolvers/native/trsreg)
