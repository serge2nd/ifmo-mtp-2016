#pragma once
#include <omp.h>
#include "../common/defs.h"
#include "../common/utils.h"
#include "../common/matrix_utils.h"

namespace ifmo_mtp_omp {
using ifmo_mtp_utils::Matrix;
using ifmo_mtp_utils::Matrix2D;
using ifmo_mtp_utils::requireMultiplying;

#define MP_DECL(id) Matrix2D<> id(Matrix<> &a, Matrix<> &b) {\
	requireMultiplying(a, b);\
	size_t m = a.nRows(), n = b.nCols(), p = a.nCols();\
	Matrix2D<> c(m, n);\
\
	omp_set_nested(OMP_NESTED);\
\
__OMP_PARALLEL \
	{\
__OMP_FOR \
		for(ssize_t i = 0; i < m; i++) {\
	_____OMP_PARALLEL \
			{\
	_____OMP_FOR \
				for(ssize_t j = 0; j < n; j++) {\
					double cell = c(i, j) = 0;\
		________OMP_PARALLEL \
					{\
		________OMP_FOR \
						for(ssize_t k = 0; k < p; k++) {\
							cell += (a(i, k) * b(k, j));\
						}\
					}\
					c(i, j) = cell;\
				}\
			}\
		}\
	}\
	return c;\
}

#define MP_DECL_COLLAPSE(id) Matrix2D<> id(Matrix<> &a, Matrix<> &b) {\
	requireMultiplying(a, b);\
	size_t m = a.nRows(), n = b.nCols(), p = a.nCols(),\
			mn = m * n;\
	Matrix2D<> c(m, n);\
\
	omp_set_nested(OMP_NESTED);\
\
__OMP_PARALLEL \
	{\
__OMP_FOR \
		for(ssize_t ij = 0; ij < mn; ij++) {\
			size_t i = ij / n, j = ij % n;\
			double cell = c(i, j) = 0;\
________OMP_PARALLEL \
			{\
________OMP_FOR \
				for(ssize_t k = 0; k < p; k++) {\
					cell += (a(i, k) * b(k, j));\
				}\
			}\
			c(i, j) = cell;\
		}\
	}\
	return c;\
}

Matrix2D<> mpManual(Matrix<> &a, Matrix<> &b) {
	requireMultiplying(a, b);
	size_t m = a.nRows(), n = b.nCols(),
			p = a.nCols(), mn = m * n;
	double initialVal = 0;
	Matrix2D<> c(m, n, &initialVal);

	size_t nThreads = omp_get_max_threads();
	size_t taskSize = mn / nThreads;

#pragma omp parallel
	{
		size_t threadNum = omp_get_thread_num();
		if (threadNum == 0 || taskSize != 0) {
			size_t start = threadNum * taskSize,
					end = start + taskSize;
			if (threadNum == nThreads - 1 || taskSize == 0)
				end = mn;
			size_t startRowIndex = start / n,
					startColIndex = start % n,
					endRowIndex = (end - 1) / n + 1,
					endColIndex = (end + n - 1) % n + 1;

			for(size_t i = startRowIndex; i < endRowIndex; i++)
				for(size_t j = startColIndex; j < endColIndex; j++)
					for(size_t k = 0; k < p; k++)
						c(i, j) += (a(i, k) * b(k, j));
		}
	}

	return c;
}
}
