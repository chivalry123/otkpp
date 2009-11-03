
SET(INST_HEADERS "../linalg/LBFGSUpdater.h"
                 "../linalg/BFGSUpdater.h"
                 "../linalg/cholesky.hpp"
                 "../linalg/InvLBFGSUpdater.h"
                 "../linalg/QuasiNewtonUpdater.h"
                 "../linalg/SR1Updater.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/linalg)
