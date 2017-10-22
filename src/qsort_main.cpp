#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <mpi.h>
#include "common/defs.h"
#include "common/utils.h"

using std::cout;
using std::endl;
using std::fstream;
using std::ios_base;
using std::runtime_error;
using std::invalid_argument;
using std::exception;
using std::vector;
using std::string;
using std::stoi;
using std::to_string;
using ifmo_mtp_utils::checkStream;
using ifmo_mtp_utils::qsort;
using ifmo_mtp_utils::partition;
using ifmo_mtp_utils::getSetBitsNum;
using ifmo_mtp_utils::rshift;
#define DEFAULT_TAG 0
#define ELEM_TYPE MPI_INT
typedef int ElemType;

#define N_ARGS 3
#define TEST_N_ARGS 4

static bool checkArgs(int argc, char** argv, int &testSize) {
	const char* usage = "Usage: program <input_array_file> <output_file>\n"
						"       program test <test_size> <output_file>";
	const char* incorrectTestSizeMsg = "Incorrect test size ";
	if (argc != N_ARGS && !(argc == TEST_N_ARGS && strcmp(argv[1], "test") == 0)) {
		cout << usage << endl;
		return false;
	}

	if (argc == TEST_N_ARGS) {
		string testSizeStr = string(argv[2]);
		try {
			testSize = stoi(testSizeStr);
			if (testSize < 1)
				throw invalid_argument("");
		} catch(const exception &) {
			cout << incorrectTestSizeMsg << endl;
			MPI_HALT(1);
		}
	}

	return true;
}

static void mpiGetCount(int source, int* bufSize, MPI_Datatype dataType, int tag, MPI_Status* mpiStatus) {
	MPI_Probe(source, tag, MPI_COMM_WORLD, mpiStatus);
	MPI_Get_count(mpiStatus, dataType, bufSize);
}

static ElemType* split(ElemType* arr, int size, int* bufSize, int nProcs, int rank, MPI_Status* mpiStatus) {
	ElemType* buf = NULL;
	if (rank == 0) {
		int rest = size % nProcs;
		int compl = (rest == 0 || size < nProcs) ? 0 : (nProcs - rest);
		*bufSize = (size + compl)/nProcs;

		if (*bufSize == 0) *bufSize = size;
		buf = new ElemType[*bufSize];
		std::copy(arr, arr + *bufSize, buf);

		int i = 1;
		for (int start = *bufSize; start < size; start += *bufSize) {
			int end = start + *bufSize;
			if (end > size) end = size;
			MPI_Send(arr + start, end - start, ELEM_TYPE, i++, DEFAULT_TAG, MPI_COMM_WORLD);
		}
		
		for (; i < nProcs; i++)
			MPI_Send(arr, 0, ELEM_TYPE, i, DEFAULT_TAG, MPI_COMM_WORLD);
	}
	else {
		mpiGetCount(0, bufSize, ELEM_TYPE, DEFAULT_TAG, mpiStatus);
		buf = new ElemType[*bufSize];
		MPI_Recv(buf, *bufSize, ELEM_TYPE, 0, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
	}
	return buf;
}

void bcastPivot(ElemType* buf, int bufSize, ElemType* ppivot, int rank, int masterRank, int stageMask, MPI_Status* mpiStatus) {
	if (rank == masterRank) {
		*ppivot = buf[bufSize/2];
		int lastRank = masterRank | stageMask;
		for (int i = rank + 1; i <= lastRank; i++)
			MPI_Send(ppivot, 1, ELEM_TYPE, i, DEFAULT_TAG, MPI_COMM_WORLD);
	} else {
		MPI_Recv(ppivot, 1, ELEM_TYPE, masterRank, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
	}
}

ElemType* exchange(ElemType* buf, int bufSize, int p,
	int* newBufSize, int* recvdSize, int rank, int partnerRank, int stage, MPI_Status* mpiStatus)
{
	ElemType* newBuf;
	if (((rank >> (stage - 1)) & 1) == 0) {
		MPI_Send(buf + p, bufSize - p, ELEM_TYPE, partnerRank, DEFAULT_TAG, MPI_COMM_WORLD);
		mpiGetCount(partnerRank, recvdSize, ELEM_TYPE, DEFAULT_TAG, mpiStatus);
		*newBufSize = p + *recvdSize; newBuf = new ElemType[*newBufSize + *recvdSize];
		MPI_Recv(newBuf + *newBufSize, *recvdSize, ELEM_TYPE, partnerRank, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		std::merge(buf, buf + p, newBuf + *newBufSize, newBuf + *newBufSize + *recvdSize, newBuf);
	} else {
		mpiGetCount(partnerRank, recvdSize, ELEM_TYPE, DEFAULT_TAG, mpiStatus);
		*newBufSize = bufSize - p + *recvdSize; newBuf = new ElemType[*newBufSize + *recvdSize];
		MPI_Recv(newBuf + *newBufSize, *recvdSize, ELEM_TYPE, partnerRank, DEFAULT_TAG, MPI_COMM_WORLD, mpiStatus);
		MPI_Send(buf, p, ELEM_TYPE, partnerRank, DEFAULT_TAG, MPI_COMM_WORLD);
		std::merge(buf + p, buf + bufSize, newBuf + *newBufSize, newBuf + *newBufSize + *recvdSize, newBuf);
	}
	return newBuf;
}

int qsort_main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int nProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	ElemType *buf, *result = NULL; int bufSize;
	int stageMask = nProcs - 1; clock_t startTime = 0;
	MPI_Status mpiStatus; int *finalCounts = NULL, *displs = NULL;

	vector<ElemType> vec; int size = 0;
	const char *inputFilename, *outFilename;

	if (rank == 0) {
		if (!checkArgs(argc, argv, size) || getSetBitsNum(nProcs) != 1) {
			MPI_HALT(1);
		}
		if (argc == TEST_N_ARGS) {
			outFilename = argv[3];
			for (int i = 0; i < size; i++)
				vec.push_back(rand());
		}
		else {
			inputFilename = argv[1];
			outFilename = argv[2];
			try {
				fstream fin(inputFilename, ios_base::in);
				checkStream(inputFilename, fin);
				ElemType x;
				while (fin >> x) vec.push_back(x);
				size = vec.size();
			}
			catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}

		startTime = clock();
	} else vec.push_back(0);
	
	buf = split(&vec[0], size, &bufSize, nProcs, rank, &mpiStatus);
	vec.clear();

	if (nProcs == 1) {
		qsort(buf, 0, size);
	} else while (stageMask != 0) {
		int stage = getSetBitsNum(stageMask);
		int masterRank = (rank >> stage) << stage;
		int nextStageMask = rshift(stageMask, 1);
		int partnerRank = rank ^ (nextStageMask + 1);
		
		if (stageMask == nProcs - 1)
			qsort(buf, 0, bufSize);
		ElemType pivot = 0;
		bcastPivot(buf, bufSize, &pivot, rank, masterRank, stageMask, &mpiStatus);

		int p = partition(buf, 0, bufSize, pivot);
		int recvdSize, newBufSize;
		ElemType* newBuf = exchange(buf, bufSize, p, &newBufSize, &recvdSize, rank, partnerRank, stage, &mpiStatus);

		delete[] buf; buf = newBuf;
		bufSize = newBufSize;
		stageMask = nextStageMask;
	}

	if (rank == 0) {
		result = new ElemType[size];
		finalCounts = new ElemType[nProcs];
		displs = new ElemType[nProcs];
	}
	MPI_Gather(&bufSize, 1, MPI_INT, finalCounts, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		finalCounts[0] = bufSize; displs[0] = 0;
		for (int i = 1; i < nProcs; i++)
			displs[i] = displs[i - 1] + finalCounts[i - 1];
	}

	MPI_Gatherv(buf, bufSize, ELEM_TYPE, result, finalCounts, displs, ELEM_TYPE, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		cout << " " << clock() - startTime;
		delete[] finalCounts; delete[] displs;

		if (strcmp(outFilename, "0") != 0) {
			try {
				fstream fout(outFilename, ios_base::out);
				checkStream(outFilename, fout);
				for (int i = 0; i < size; i++)
					if (!(fout << result[i] << " "))
						throw runtime_error(string("Cannot write result[") + to_string(i) + "]");
			}
			catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}

		delete[] result;
	}

	delete[] buf;
	MPI_Finalize();
	return 0;
}
