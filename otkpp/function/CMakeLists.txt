
SET(INST_HEADERS "EvalCountable.h"
                 "FDiffGradientEvaluator.h"
                 "FDiffHessianEvaluator.h"
                 "FunctionEvaluator.h"
                 "Function.h"
                 "GradientEvaluator.h"
                 "HessianEvaluator.h"
                 "SymbolicFunctionEvaluator.h"
                 "SymbolicGradientEvaluator.h"
                 "SymbolicHessianEvaluator.h")

INSTALL(FILES ${INST_HEADERS} DESTINATION include/otkpp/function)
