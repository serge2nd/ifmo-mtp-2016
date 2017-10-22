#pragma once
#include <functional>

// **********************
// *** Utility macros ***
// **********************
#if defined __GNUC__
	#define PRAGMA(x) _Pragma(#x)
	#if defined __clang__
		typedef long ssize_t;
	#endif
#elif defined _MSC_VER
	#include <basetsd.h>
	typedef SSIZE_T ssize_t;
	#define PRAGMA(x) __pragma(x)
    #pragma warning(disable: 4003 4018 4244)
#else
	#error Neither MSVC nor GNUC is used (unsupported)
#endif

#define RAND_LIM (RAND_MAX + 1.0)
#define DBL_PREC 1e-5
template<class T>
using BinPred = std::function<bool(const T&, const T&)>;

// *****************************
// *** OpenMP & MPI commands ***
// *****************************
#define OMP_PARALLEL(params) PRAGMA(omp parallel params)
#define OMP_FOR(params) PRAGMA(omp for params)
#define MPI_HALT(code) MPI_Abort(MPI_COMM_WORLD, code)
