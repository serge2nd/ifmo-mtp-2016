#pragma once
#include <vector>
#include <mpi.h>
#include "matrix_utils.h"
#include "mpi_utils.h"

namespace ifmo_mtp_utils {
using std::vector;

/**
* Shares slices of the given matrix between processes
*/
template<class T>
void mpiSplit(Matrix<T> &matrSlice, MPI_Datatype type, int nRows, int nCols,
	int* sliceSize, int* sliceStart, int rank, int nProcs, MPI_Status* mpiStatus)
{
	if (rank == 0) {
		*sliceStart = 0;
		*sliceSize = int(splitSize(nRows, nProcs));
		int *startInds = new int[nProcs - 1],
			*sizes = new int[nProcs - 1];

		vector<MPI_Request> reqVec;
		vector<MPI_Status> statusVec;
		int i = 1;
		for (int start = *sliceSize; start < nRows; i++, start += *sliceSize) {
			int end = start + *sliceSize;
			if (end > nRows) end = nRows;
			int realSize = end - start;
			
			startInds[i - 1] = start; sizes[i - 1] = realSize;
			isend(startInds + i - 1, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
			isend(sizes + i - 1, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
			for (int k = start; k < end; k++) {
				isend(&matrSlice(k), nCols, type, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
			}
		}
		for (; i < nProcs; i++) {
			isend(sliceStart, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
		}

		if (reqVec.size() > 0)
			MPI_Waitall(int(reqVec.size()), &reqVec[0], &statusVec[0]);
		delete[] startInds; delete[] sizes;
		return;
	}

	MPI_Recv(sliceStart, 1, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
	if (*sliceStart > 0) {
		MPI_Recv(sliceSize, 1, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		matrSlice = Matrix2D<T>(*sliceSize, nCols);
		for (int k = 0; k < *sliceSize; k++) {
			MPI_Recv(&matrSlice(k), nCols, type, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		}
	}
}
}