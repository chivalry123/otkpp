
#ifndef LMBM_UTILS_H

/*#if defined(__alpha__) || defined(__sparc64__) || defined(__x86_64__) || define$
typedef int integer;
typedef unsigned int uinteger;
#else
typedef long int integer;
typedef unsigned long int uinteger;
#endif
typedef float real_;
typedef double doublereal;*/

#ifdef __cplusplus
extern "C"
{
#endif
int lmbmu_(int *n, int *na, int *mcu, int *mc, int *nw, double *x,
           double *xr, double *f, double *rpar, int *ipar,
           int *iout, float *time, float *rtim, double *w);
#ifdef __cplusplus
}
#endif

#define LMBM_UTILS_H

#endif
