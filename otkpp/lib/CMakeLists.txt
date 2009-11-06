
SET(INST_HEADERS "Cloneable.h"
                 "OTK.h")

SET(OTKPP_SRCS "../constraints/BoundConstraints.cpp"
               "../function/EvalCountable.cpp"
               "../function/FDiffGradientEvaluator.cpp"
               "../function/FDiffHessianEvaluator.cpp"
               "../function/Function.cpp"
               "../function/FunctionEvaluator.cpp"
               "../function/GradientEvaluator.cpp"
               "../function/HessianEvaluator.cpp"
               "../linalg/BFGSUpdater.cpp"
               "../linalg/LBFGSUpdater.cpp"
               "../linalg/InvLBFGSUpdater.cpp"
               "../linalg/SR1Updater.cpp"
               "../localsolvers/AbstractGradientSolver.cpp"
               "../localsolvers/ExternalSolver.cpp"
               "../localsolvers/Solver.cpp"
               "../localsolvers/native/linmin/Fletcher.cpp"
               "../localsolvers/native/linmin/LineMinimizer.cpp"
               "../localsolvers/native/linmin/MoreThuente.cpp"
               "../localsolvers/native/linmin/PolyFit.cpp"
               "../localsolvers/native/trsreg/DoglegSolver.cpp"
               "../localsolvers/native/trsreg/SteihaugSolver.cpp"
               "../localsolvers/native/trsreg/TrustRegionSolver.cpp"
               "../localsolvers/native/ConjGradMT.cpp"
               "../localsolvers/native/DoglegBFGS.cpp"
               "../localsolvers/native/GradientSolver.cpp"
               "../localsolvers/native/HookeJeeves.cpp"
               "../localsolvers/native/LinminBFGS.cpp"
               "../localsolvers/native/MNewton.cpp"
               "../localsolvers/native/NativeSolver.cpp"
               "../localsolvers/native/PARTAN.cpp"
               "../localsolvers/native/SteihaugSR1.cpp"
               "../stopcrit/CompoundStoppingCriterion.cpp"
               "../stopcrit/FDistToMinTest.cpp"
               "../stopcrit/GradNormTest.cpp"
               "../stopcrit/MaxNumIterTest.cpp"
               "../stopcrit/StoppingCriterion.cpp"
               "../stopcrit/XDistToMinTest.cpp"
               "../testproblems/MGHTestFunction.cpp")

SET(OTKPP_GSL_SRCS "../localsolvers/gslsolvers/GSLFSolver.cpp"
                   "../localsolvers/gslsolvers/GSLFDFSolver.cpp"
                   "../localsolvers/gslsolvers/GSLUtils.cpp")

IF(WITH_GSL)
  SET(OTKPP_SRCS ${OTKPP_SRCS} ${OTKPP_GSL_SRCS})
ENDIF(WITH_GSL)

SET(OTKPP_MATHEVAL_SRCS "../function/SymbolicFunctionEvaluator.cpp"
                        "../function/SymbolicGradientEvaluator.cpp"
                        "../function/SymbolicHessianEvaluator.cpp")

IF(WITH_LIBMATHEVAL)
  SET(OTKPP_SRCS ${OTKPP_SRCS} ${OTKPP_MATHEVAL_SRCS})
ENDIF(WITH_LIBMATHEVAL)

SET(OTKPP_FORTRAN_SRCS "../localsolvers/lbfgsb/LBFGSB.cpp"
                       "../localsolvers/lmbm/func_wrapper.cpp"
                       "../localsolvers/lmbm/LMBM.cpp"
                       "../localsolvers/newuoa/NEWUOA.cpp"
                       "../localsolvers/newuoa/func_wrapper.cpp"
                       "../localsolvers/lbfgsb/routines.f"
                       "../localsolvers/lmbm/lmbm.f"
                       "../localsolvers/lmbm/lmsub.f"
                       "../localsolvers/lmbm/matcal.f"
                       "../localsolvers/lmbm/tlmbm.f"
                       "../localsolvers/lmbm/tnsunc.f"
                       "../localsolvers/newuoa/bigden.f"
                       "../localsolvers/newuoa/biglag.f"
                       "../localsolvers/newuoa/newuoa.f"
                       "../localsolvers/newuoa/newuob.f"
                       "../localsolvers/newuoa/trsapp.f"
                       "../localsolvers/newuoa/update.f")

INCLUDE_DIRECTORIES(${OTKPP_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})

ADD_LIBRARY(otkpp SHARED OTK.cpp ${OTKPP_SRCS})
INSTALL(TARGETS otkpp LIBRARY DESTINATION lib)

IF(WITH_FORTRAN)
  ADD_LIBRARY(otkpp_fortran SHARED ${OTKPP_FORTRAN_SRCS})
  INSTALL(TARGETS otkpp_fortran LIBRARY DESTINATION lib)
  SET_TARGET_PROPERTIES(otkpp_fortran PROPERTIES LINKER_LANGUAGE CXX)
ENDIF(WITH_FORTRAN)

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/lib)
