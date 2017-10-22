#include <iostream>
#include "graph/graph_mpi.h"

using std::cout;
using std::setw;
using std::endl;
using std::string;
using std::to_string;
using std::stod;
using std::stoi;
using std::vector;
using std::fstream;
using std::istream;
using std::ios_base;
using std::out_of_range;
using std::invalid_argument;
using std::logic_error;
using std::runtime_error;
using std::exception;
using ifmo_mtp_utils::checkStream;
using ifmo_mtp_utils::require;
using ifmo_mtp_utils::heapify;
using ifmo_mtp_utils::decreaseKey;
using ifmo_mtp_utils::mpiGetDispls;
using ifmo_mtp_graph::WeightType;
using ifmo_mtp_graph::Edge;
using ifmo_mtp_graph::split;
using ifmo_mtp_graph::parseDot;
using ifmo_mtp_graph::writeDot;
using ifmo_mtp_graph::generateGraph;
using ifmo_mtp_graph::mpiDijkstraA;
using ifmo_mtp_graph::mpiDijkstraQ;

#define N_ARGS 5
#define TEST_N_ARGS 7

static bool checkArgs(int argc, char** argv, int &source, int &nNodes,
	WeightType &startWeight, WeightType &endWeight, double &density)
{
	const char* usage = "Usage: program <dot_file> <source> <dot_output> <weights_output>\n"
						"       program <n_vertices> <start_weight> <end_weight> <density> <dot_output> <weights_output>";
	const char* incorrectSourceMsg = "Source must be non-negative int, given ";
	const char* incorrectTestParamsMsg = "Incorrect test params: ";
	if (argc != N_ARGS && argc != TEST_N_ARGS) {
		cout << usage << endl;
		return false;
	}

	if (argc == N_ARGS) {
		string sourceStr(argv[2]);
		try {
			source = stoi(sourceStr);
			if (source < 0)
				throw out_of_range("");
		} catch (const logic_error &) {
			cout << incorrectSourceMsg << sourceStr << endl;
			return false;
		}
	} else {
		string nNodesStr(argv[1]), startWeightStr(argv[2]), endWeightStr(argv[3]), densityStr(argv[4]);
		try {
			nNodes = stoi(nNodesStr);
			startWeight = stoi(startWeightStr);
			endWeight = stoi(endWeightStr);
			density = stod(densityStr);
			if (nNodes < 1 || startWeight < 0 || startWeight > endWeight || density > 1 || density <= 0)
				throw out_of_range("");
		} catch (const logic_error &) {
			cout << incorrectTestParamsMsg
				<< nNodesStr << " "
				<< startWeightStr << " "
				<< endWeightStr << " "
				<< densityStr << endl;
			return false;
		}
	}

	return true;
}

int dijkstra_main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	srand(unsigned(time(NULL))), rand();

	int nProcs;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// *** Necessary variables ***
	vector<Edge> *adjLists = NULL, *adjListsCopy; int nNodes = 0, startNode = 0, endNode = 0, source = 0;
	int *globalp = NULL; WeightType *globald = NULL; WeightType startWeight = 0, endWeight = 0;
	MPI_Status mpiStatus; double density = 0;

	// *** Parsing arguments ***
	const char *dotFilename, *outDotFilename, *outWeightsFilename;
	if (rank == 0) {
		if (!checkArgs(argc, argv, source, nNodes, startWeight, endWeight, density)) {
			MPI_HALT(1);
		}
		if (argc == TEST_N_ARGS) {
			adjLists = generateGraph(nNodes, density, startWeight, endWeight);
			outDotFilename = argv[5]; outWeightsFilename = argv[6];
		} else {
			dotFilename = argv[1];
			outDotFilename = argv[3]; outWeightsFilename = argv[4];
			try {
				fstream fin(dotFilename, ios_base::in);
				checkStream(dotFilename, fin);
				adjLists = parseDot(fin, nNodes);
				if (source >= nNodes)
					throw out_of_range("Source node is out of range: "
						+ to_string(source));
				fin.close();
			} catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}

		globalp = new int[nNodes]; globald = new WeightType[nNodes];
	}

	// Bcast nodes count and source & create adjacency lists copy
	MPI_Bcast(&nNodes, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&source, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		adjListsCopy = ifmo_mtp_graph::copy(adjLists, nNodes);
	else
		adjLists = new vector<Edge>[nNodes];
	
	// *** Split & run algorithm ***
	split(adjLists, nNodes, &startNode, &endNode, rank, nProcs, &mpiStatus);
	clock_t startTime = clock();
	mpiDijkstraQ(adjLists, nNodes, source, startNode, endNode, globalp, globald, rank, nProcs);
	delete[] adjLists;
	
	// *** Output results ***
	if (rank == 0) {
		cout << " " << clock() - startTime;

		// Highlight the shortest paths
		for (int u = 0; u < nNodes; u++) {
			int degree = int(adjListsCopy[u].size());
			for (int j = 0; j < degree; j++) {
				Edge &e = adjListsCopy[u][j];
				if (globalp[e.dest] == u) e.hlighted = 1;
			}
		}

		// Write DOT file and path weights
		if (strcmp(outDotFilename, "0") != 0) {
			try {
				fstream fout(outDotFilename, ios_base::out);
				checkStream(outDotFilename, fout);
				writeDot(fout, adjListsCopy, nNodes);
				fout.close();

				fout = fstream(outWeightsFilename, ios_base::out);
				checkStream(outWeightsFilename, fout);
				for (int u = 0; u < nNodes; u++)
					require(fout << globalp[u] << " ",
						"Cannot write path weight for vertex " + to_string(u));
				fout.close();
			}
			catch (const exception &e) {
				cout << e.what() << endl;
				MPI_HALT(1);
			}
		}
		
		delete[] adjListsCopy;
		delete[] globalp; delete[] globald;
	}

	MPI_Finalize();
	return 0;
}