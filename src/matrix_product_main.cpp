#include <iostream>
#include "matrix_product/mp_omp_test.h"

using std::string;
using std::stoi;
using std::exception;
using std::logic_error;
using std::out_of_range;
using std::cout;
using std::setw;
using std::fixed;
using std::setprecision;
using std::fstream;
using std::endl;
using std::ios_base;
using ifmo_mtp_utils::Matrix2D;
using ifmo_mtp_utils::rndMatrix;
using ifmo_mtp_utils::loadMatrix;
using ifmo_mtp_utils::checkStream;
using namespace ifmo_mtp_omp;

#define N_ARGS 5
#define TEST_MODE_N_ARGS 2

static bool checkArgs(int argc, char** argv, int &nThreads) {
	const char* usage = "Usage: program <n_threads> <matrix_file1> <matrix_file2> <dest_file>\n"
						"       program <n_threads>";
	const char* incorrectNThreadsMsg = "Incorrect threads count ";

	// Check number of arguments
	if (argc != N_ARGS && argc != TEST_MODE_N_ARGS) {
		cout << usage << endl;
		return false;
	}

	// Check number of threads
	string nThreadsStr(argv[1]);
	try {
		nThreads = stoi(nThreadsStr);
		if (nThreads < 1)
			throw out_of_range("");
	} catch (const logic_error &) {
		cout << incorrectNThreadsMsg << nThreadsStr << endl;
		return false;
	}

	return true;
}

static int testMpSpeed() {
	const int nAttempts = 1;
	const double leftBnd = -1000, rightBnd = 1000;
	Matrix2D<> a, b; clock_t* times;
	char* labels[] = LABELS; int testSizes[][3] = TEST_SIZES;

	cout << labels[0];
	for(int i = 1; i < NUM_IMPLS + 1; i++)
		cout << "," << labels[i];
	cout << endl;

	for(int n = 0; n < TEST_SIZES_COUNT; n++) {
		int n1 = testSizes[n][0], n2 = testSizes[n][1], n3 = testSizes[n][2];
		for(int k = 0; k < nAttempts; k++) {
			a.slf = rndMatrix(n1, n2, leftBnd, rightBnd);
			b.slf = rndMatrix(n2, n3, leftBnd, rightBnd);
			times = getMulTime(a, b);
			cout << "#" << k << "_" << n1 << "x" << n2 << "x" << n3;
			for(int i = 0; i < NUM_IMPLS; i++)
				cout << "," << times[i];
			cout << endl;
		}
	}

	return 0;
}

static int testMp() {
	srand(time(NULL)), rand();
	/*if (!testMpResult())
		return 1;
	cout << "Testing correct results finished." << endl;*/
	return testMpSpeed();
}

int matrix_product_main(int argc, char **argv) {
	int nThreads;
	if (!checkArgs(argc, argv, nThreads)) {
		return 1;
	}

	omp_set_dynamic(0);
	omp_set_num_threads(nThreads);
	if (argc == TEST_MODE_N_ARGS)
		return testMp();

	const char* filename1 = argv[2];
	const char* filename2 = argv[3];
	const char* resultFilename = argv[4];
	Matrix2D<> a, b;

	// Read matrices from the given files
	try {
		fstream matrixFile1(filename1, ios_base::in);
		fstream matrixFile2(filename2, ios_base::in);

		checkStream(filename1, matrixFile1);
		loadMatrix(matrixFile1, a);
		matrixFile1.close();

		checkStream(filename2, matrixFile2);
		loadMatrix(matrixFile2, b);
		matrixFile2.close();
	} catch (const exception &e) {
		cout << string("Unable to read matrices: ") + e.what() << endl;
		return 1;
	}

	try {
		Matrix2D<> c = mpManual(a, b);
		a.slf = Matrix2D<>(); b.slf = Matrix2D<>();
		fstream resultFile(resultFilename, ios_base::out);

		checkStream(resultFilename, resultFile);
		resultFile << '\t' << c;
		resultFile.close();
	} catch (const exception &e) {
		cout << string("Unable to put matrix product: ") + e.what() << endl;
		return 1;
	}

	return 0;
}
