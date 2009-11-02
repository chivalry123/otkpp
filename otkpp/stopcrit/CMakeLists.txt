
SET(INST_HEADERS "CompoundStoppingCriterion.h"
                 "FDistToMinTest.h"
                 "GradNormTest.h"
                 "MaxNumIterTest.h"
                 "StoppingCriterion.h"
                 "XDistToMinTest.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/stopcrit)
