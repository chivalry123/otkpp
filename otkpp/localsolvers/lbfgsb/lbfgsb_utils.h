
#ifndef LBFGSB_UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif
extern int setulb_(int *n, int *m, double *x, double *l, double *u,
                   int *nbd, double *f, double *g, double *factr,
                   double *pgtol, double *wa, int *iwa, char *task,
                   int *iprint, char *csave, int *lsave,
                   int *isave, double *dsave, int, int);
#ifdef __cplusplus
}
#endif

#define LBFGSB_UTILS_H

#endif
