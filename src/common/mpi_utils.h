#pragma once
#include <vector>
#include <mpi.h>

namespace ifmo_mtp_utils {
using std::vector;
#define DEFAULT_TAG 0

/**
 * Gets recv buffer size with {@code MPI_Probe()} and {@code MPI_Get_count()}
 */
void mpiGetCount(int source, int* bufSize, MPI_Datatype type, int tag, MPI_Comm comm, MPI_Status* status) {
	MPI_Probe(source, tag, comm, status);
	MPI_Get_count(status, type, bufSize);
}

/**
 * Adds necessary info objects to the vectors and sends via {@code MPI_Isend()}
 */
void isend(const void* buf, int count, MPI_Datatype type, int dest,
	int tag, MPI_Comm comm, vector<MPI_Request> &reqVec, vector<MPI_Status> &statusVec)
{
	reqVec.resize(reqVec.size() + 1);
	statusVec.resize(statusVec.size() + 1);
	MPI_Isend(buf, count, type, dest, tag, comm, &reqVec.back());
}

/**
 * Displacements from buffer sizes for {@code MPI_Gatherv()}
 */
int* mpiGetDispls(int nProcs, int* sliceSizes, int* displs) {
	displs[0] = 0;
	for (int i = 1; i < nProcs; i++)
		displs[i] = displs[i - 1] + sliceSizes[i - 1];
	return displs;
}
}