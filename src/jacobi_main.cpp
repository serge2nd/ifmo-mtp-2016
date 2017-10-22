#include <iostream>
#include <algorithm>
#include <vector>
#include "common/mpi_matrix.h"
#include "jacobi/jacobi_utils.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::stod;
using std::vector;
using std::fstream;
using std::ios_base;
using std::out_of_range;
using std::invalid_argument;
using std::logic_error;
using std::runtime_error;
using std::exception;
using ifmo_mtp_utils::e;
using ifmo_mtp_utils::mp;
using ifmo_mtp_utils::diag;
using ifmo_mtp_utils::mNorm;
using ifmo_mtp_utils::diagInv;
using ifmo_mtp_utils::isend;
using ifmo_mtp_utils::mpiGetCount;
using ifmo_mtp_utils::mpiGetDispls;
using ifmo_mtp_utils::mpiSplit;
using ifmo_mtp_utils::checkStream;
using ifmo_mtp_utils::loadMatrix;
using ifmo_mtp_utils::Matrix2D;
using ifmo_mtp_utils::require;
using namespace ifmo_mtp_jacobi;

#define N_ARGS 6
#define TEST_N_ARGS 7

static bool checkArgs(int argc, char** argv, int &n, double &rangeStart,
	double &rangeEnd, double &q, double &epsilon, int &nIters)
{
	const char* usage = "Usage: program <matrix_file> <init_vector_file> <epsilon> <n_iters> <output_file>\n"
						"       program <square_size> <range_start> <range_end> <B_norm> <epsilon> <n_iters>";
	const char* incorrectTestParamsMsg = "Incorrect test params";
	const char* incorrectNumbersMsg = "epsilon must be >0 and n_iters must be >=0";
	if (argc != N_ARGS && argc != TEST_N_ARGS) {
		cout << usage << endl;
		return false;
	}

	string epsilonStr, nItersStr;
	if (argc == N_ARGS) {
		epsilonStr = string(argv[3]), nItersStr = string(argv[4]);
	} else {
		epsilonStr = string(argv[5]), nItersStr = string(argv[6]);
		string nStr(argv[1]), rangeStartStr(argv[2]),
			rangeEndStr(argv[3]), qStr(argv[4]);

		try {
			n = stoi(nStr);
			rangeStart = stod(rangeStartStr);
			rangeEnd = stod(rangeEndStr);
			q = stod(qStr);
			if (n <= 0 || q < 0)
				throw out_of_range("");
		}
		catch (const logic_error &) {
			cout << incorrectTestParamsMsg << endl;
			return false;
		}
	}

	try {
		epsilon = stod(epsilonStr);
		nIters = stoi(nItersStr);
		if (epsilon <= 0 || nIters < 0)
			throw out_of_range("");
		if (nIters == 0) nIters = INT_MAX;
	}
	catch (const logic_error &) {
		cout << incorrectNumbersMsg << endl;
		return false;
	}

	return true;
}

int jacobi_main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	srand(unsigned(time(NULL))), rand();

	int nProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// *** Necessary variables ***
	Matrix2D<> matrSlice, xMatr, testX; double rangeStart, rangeEnd, *nextX; int nIters = 0;
	int totalSize, sliceSize = 0, sliceStart = 0, *sliceSizes = NULL, *displs = NULL;
	MPI_Status mpiStatus; double q = 0, epsilon = 0, currEps = DBL_MAX, globalEps = 0;

	// *** Parsing arguments ***
	char *matrFilename, *xFilename, *outFilename;
	if (rank == 0) {
		if (!checkArgs(argc, argv, totalSize, rangeStart, rangeEnd, q, epsilon, nIters)) {
			MPI_HALT(1);
		}

		if (argc == TEST_N_ARGS) {
			xMatr.slf = rndMatrix(1, totalSize, 0, 1);
			matrSlice.slf = Matrix2D<>(totalSize, totalSize + 1);
			generateLSE(totalSize, rangeStart, rangeEnd, q, matrSlice, testX);
		} else {
			matrFilename = argv[1];
			xFilename = argv[2];
			outFilename = argv[5];
			try {
				fstream fin(matrFilename, ios_base::in);
				checkStream(matrFilename, fin);
				size_t m, n;
				require(fin >> m >> n, "Reading matrix size failed");
				if (n != m + 1)
					throw invalid_argument("Matrix of linear equations system is required, given "
						+ to_string(m) + "x" + to_string(n));
				loadMatrix(fin, matrSlice, true, m, n);
				fin.close();

				fin = fstream(xFilename, ios_base::in);
				checkStream(xFilename, fin);
				loadMatrix(fin, xMatr, true, 1, m);
				fin.close();
			}
			catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}

		sliceSizes = new int[nProcs];
		displs = new int[nProcs];
	}
	
	// *** Split LSE matrix & broadcast parameters ***
	totalSize = int(matrSlice.nRows());
	MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	mpiSplit(matrSlice, MPI_DOUBLE, totalSize, totalSize + 1, &sliceSize, &sliceStart, rank, nProcs, &mpiStatus);
	if (rank != 0)
		xMatr.slf = Matrix2D<>(1, totalSize);
	MPI_Bcast(&xMatr(0), totalSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	nextX = new double[sliceSize];
	MPI_Bcast(&q, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&epsilon, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&nIters, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// *** Start iterations ***
	int iters = 0; clock_t startTime = clock();
	while (iters < nIters && currEps > epsilon) {
		currEps = doIteration(matrSlice, &xMatr(0), nextX, totalSize, sliceStart, sliceSize);
		MPI_Gather(&sliceSize, 1, MPI_INT, sliceSizes, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (rank == 0)
			mpiGetDispls(nProcs, sliceSizes, displs);

		MPI_Gatherv(nextX, sliceSize, MPI_DOUBLE, &xMatr(0), sliceSizes, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(&xMatr(0), totalSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		MPI_Allreduce(&currEps, &globalEps, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
		currEps = globalEps/(1 - q);
		iters++;
	}
	delete[] nextX;
	matrSlice.slf = Matrix2D<>();
	
	// *** Write results to file if required ***
	if (rank == 0) {
		cout << " " << clock() - startTime;
		delete[] sliceSizes;
		delete[] displs;

		if (argc == N_ARGS) {
			try {
				fstream fout(outFilename, ios_base::out);
				checkStream(outFilename, fout);
				for (int i = 0; i < totalSize; i++)
					require(fout << '\t' << xMatr(0, i) << endl,
						"Cannot write x[" + to_string(i) + "]");
				fout.close();
			}
			catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}
	}

	MPI_Finalize();
	return 0;
}
