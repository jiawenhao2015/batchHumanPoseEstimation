//
// MATLAB Compiler: 6.3 (R2016b)
// Date: Wed Apr 12 21:00:15 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:l1_ls" "-T" "link:lib" "-d"
// "E:\l1_ls\for_testing" "-v" "E:\l1_ls.m" 
//

#ifndef __l1_ls_h
#define __l1_ls_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_l1_ls
#define PUBLIC_l1_ls_C_API __global
#else
#define PUBLIC_l1_ls_C_API /* No import statement needed. */
#endif

#define LIB_l1_ls_C_API PUBLIC_l1_ls_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_l1_ls
#define PUBLIC_l1_ls_C_API __declspec(dllexport)
#else
#define PUBLIC_l1_ls_C_API __declspec(dllimport)
#endif

#define LIB_l1_ls_C_API PUBLIC_l1_ls_C_API


#else

#define LIB_l1_ls_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_l1_ls_C_API 
#define LIB_l1_ls_C_API /* No special import/export declaration */
#endif

extern LIB_l1_ls_C_API 
bool MW_CALL_CONV l1_lsInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_l1_ls_C_API 
bool MW_CALL_CONV l1_lsInitialize(void);

extern LIB_l1_ls_C_API 
void MW_CALL_CONV l1_lsTerminate(void);



extern LIB_l1_ls_C_API 
void MW_CALL_CONV l1_lsPrintStackTrace(void);

extern LIB_l1_ls_C_API 
bool MW_CALL_CONV mlxL1_ls(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_l1_ls
#define PUBLIC_l1_ls_CPP_API __declspec(dllexport)
#else
#define PUBLIC_l1_ls_CPP_API __declspec(dllimport)
#endif

#define LIB_l1_ls_CPP_API PUBLIC_l1_ls_CPP_API

#else

#if !defined(LIB_l1_ls_CPP_API)
#if defined(LIB_l1_ls_C_API)
#define LIB_l1_ls_CPP_API LIB_l1_ls_C_API
#else
#define LIB_l1_ls_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_l1_ls_CPP_API void MW_CALL_CONV l1_ls(int nargout, mwArray& x, mwArray& status, mwArray& history, const mwArray& A, const mwArray& varargin);

#endif
#endif
