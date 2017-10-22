#pragma once
#include <iostream>
#include "../common/utils.h"
#include "../common/mpi_utils.h"
#include "graph_utils.h"

namespace ifmo_mtp_graph {
using std::cout;
using std::setw;
using std::endl;
using std::vector;
using ifmo_mtp_utils::heapify;
using ifmo_mtp_utils::decreaseKey;
using ifmo_mtp_utils::isend;
using ifmo_mtp_utils::splitSize;
using ifmo_mtp_utils::mpiGetCount;
using ifmo_mtp_utils::mpiGetDispls;
#define WEIGHT_TYPE MPI_INT

void split(vector<Edge>* adjLists, int nNodes, int* startNode, int* endNode, int rank, int nProcs, MPI_Status* mpiStatus) {
	if (rank == 0) {
		*startNode = 0;
		int sliceSize = int(splitSize(nNodes, nProcs));
		*endNode = sliceSize - *startNode;
		int *startNodes = new int[nProcs - 1],
			*endNodes = new int[nProcs - 1];

		vector<MPI_Request> reqVec;
		vector<MPI_Status> statusVec;
		int i = 1;
		for (int start = sliceSize; start < nNodes; i++, start += sliceSize) {
			int end = start + sliceSize;
			if (end > nNodes) end = nNodes;

			startNodes[i - 1] = start; endNodes[i - 1] = end;
			isend(startNodes + i - 1, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
			isend(endNodes + i - 1, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);

			for (int u = 0; u < nNodes; u++) {
				int degree = int(adjLists[u].size());

				int firstEdge = -1;
				while (++firstEdge < degree && adjLists[u][firstEdge].dest < start);
				int lastEdge = firstEdge - 1;
				while (++lastEdge < degree && adjLists[u][lastEdge].dest < end);

				int edgesCount = lastEdge - firstEdge;
				if (edgesCount > 0)
					isend(&adjLists[u][0] + firstEdge, edgesCount*sizeof(Edge), MPI_BYTE,
						i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);
				else
					isend(startNode, 1, MPI_BYTE, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);

				adjLists[u].erase(adjLists[u].begin() + firstEdge, adjLists[u].begin() + lastEdge);
			}
		}
		for (; i < nProcs; i++)
			isend(startNode, 1, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, reqVec, statusVec);

		if (reqVec.size() > 0)
			MPI_Waitall(int(reqVec.size()), &reqVec[0], &statusVec[0]);
		delete[] startNodes; delete[] endNodes;
		return;
	}

	MPI_Recv(startNode, 1, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
	if (*startNode > 0) {
		MPI_Recv(endNode, 1, MPI_INT, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		int fake;
		for (int u = 0; u < nNodes; u++) {
			int bytesCount;
			mpiGetCount(0, &bytesCount, MPI_BYTE, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);

			if (bytesCount > 1) {
				adjLists[u].resize(bytesCount/sizeof(Edge));
				MPI_Recv(&adjLists[u][0], bytesCount, MPI_BYTE, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
			} else MPI_Recv(&fake, bytesCount, MPI_BYTE, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		}
	}
}

void mpiDijkstraGather(int sliceSize, int* p, WeightType* d,
	int* globalp, WeightType* globald, int rank, int nProcs)
{
	int *sliceSizes = NULL, *displs = NULL;
	if (rank == 0) {
		sliceSizes = new int[nProcs]; displs = new int[nProcs];
	}

	MPI_Gather(&sliceSize, 1, MPI_INT, sliceSizes, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		mpiGetDispls(nProcs, sliceSizes, displs);
	MPI_Gatherv(p, sliceSize, MPI_INT, globalp, sliceSizes, displs, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gatherv(d, sliceSize, WEIGHT_TYPE, globald, sliceSizes, displs, WEIGHT_TYPE, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		delete[] sliceSizes; delete[] displs;
	}
}

void mpiDijkstraA(vector<Edge>* adjLists, int nNodes, int source, int startNode, int endNode,
	int* globalp, WeightType* globald, int rank, int nProcs)
{
	typedef struct {
		WeightType d;
		int num;
	} MinNode;

	int sliceSize = endNode - startNode;
	int *p = new int[sliceSize];
	WeightType *d = new WeightType[sliceSize];
	vector<bool> grabbed(sliceSize);

	for (int u = 0; u < sliceSize; u++) {
		p[u] = -1;
		d[u] = WEIGHT_INF - 1;
		grabbed[u] = false;
	}
	if (startNode <= source && source < endNode) {
		int localSource = source - startNode;
		d[localSource] = 0;
	}

	int grabbedNodes = 0;
	while (grabbedNodes++ < nNodes) {
		WeightType localMind = (sliceSize == 0 || grabbed[0]) ? WEIGHT_INF : d[0];
		int localMin = startNode;
		for (int u = 1; u < sliceSize; u++)
			if (!grabbed[u] && d[u] < localMind) {
				localMind = d[u];
				localMin = startNode + u;
			}
		
		int globalMin = 0;
		WeightType globalMind = 0;
		MinNode localMinStruct{localMind, localMin}, globalMinStruct{WEIGHT_INF, 0};

		MPI_Allreduce(&localMinStruct, &globalMinStruct, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
		globalMind = globalMinStruct.d;
		globalMin = globalMinStruct.num;
		if (globalMin == localMin)
			grabbed[globalMin - startNode] = true;

		if (globalMind < WEIGHT_INF - 1) {
			int degree = int(adjLists[globalMin].size());
			for (int j = 0; j < degree; j++) {
				const Edge &e = adjLists[globalMin][j];
				int local = e.dest - startNode;
				WeightType currd = d[local];

				if (currd > globalMind + e.w) {
					p[local] = globalMin;
					d[local] = globalMind + e.w;
				}
			}
		}

		/*if (rank == 0)
			cout << globalMind << " " << globalMin << endl;*/
	}

	mpiDijkstraGather(sliceSize, p, d, globalp, globald, rank, nProcs);
	delete[] p; delete[] d;
}

void mpiDijkstraQ(vector<Edge>* adjLists, int nNodes, int source, int startNode, int endNode,
	int* globalp, WeightType* globald, int rank, int nProcs)
{
	typedef struct {
		WeightType d;
		int num;
	} MinNode;

	int sliceSize = endNode - startNode;
	size_t queueSize = sliceSize;

	int *p = new int[sliceSize], *inds = new int[sliceSize];
	WeightType *d = new WeightType[sliceSize], *q = new WeightType[sliceSize];
	size_t *qInds = new size_t[sliceSize];

	for (int u = 0; u < sliceSize; u++) {
		p[u] = -1;
		d[u] = q[u] = WEIGHT_INF - 1;
		inds[u] = u;
		qInds[u] = u;
	}
	if (startNode <= source && source < endNode) {
		int localSource = source - startNode;
		d[localSource] = q[0] = 0;
		std::swap(inds[0], inds[localSource]);
		std::swap(qInds[0], qInds[localSource]);
	}

	int grabbedNodes = 0;
	while (grabbedNodes++ < nNodes) {
		WeightType localMind = (queueSize == 0) ? WEIGHT_INF : q[0];
		int localMin = inds[0] + startNode, globalMin = 0;
		WeightType globalMind = 0;
		MinNode localMinStruct{ localMind, localMin }, globalMinStruct{ WEIGHT_INF, 0 };

		MPI_Allreduce(&localMinStruct, &globalMinStruct, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
		globalMind = globalMinStruct.d;
		globalMin = globalMinStruct.num;
		if (globalMin == localMin) {
			q[0] = q[--queueSize];
			inds[0] = inds[queueSize];
			qInds[inds[0]] = 0;
			heapify(q, queueSize, 0, inds, qInds);
		}

		if (globalMind < WEIGHT_INF - 1) {
			int degree = int(adjLists[globalMin].size());
			for (int j = 0; j < degree; j++) {
				const Edge &e = adjLists[globalMin][j];
				int local = e.dest - startNode;
				WeightType currd = d[local];

				if (currd > globalMind + e.w) {
					p[local] = globalMin;
					d[local] = q[qInds[local]] = globalMind + e.w;
					decreaseKey(q, queueSize, qInds[local], inds, qInds);
				}
			}
		}

		/*if (rank == 0)
			cout << globalMind << " " << globalMin << endl;*/
	}

	mpiDijkstraGather(sliceSize, p, d, globalp, globald, rank, nProcs);
	delete[] p; delete[] d; delete[] inds;
	delete[] q; delete[] qInds;
	
}
}