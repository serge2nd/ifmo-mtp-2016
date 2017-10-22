#pragma once
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <stdexcept>

namespace ifmo_mtp_graph {
using std::vector;
using std::string;
using std::stoi;
using std::istream;
using std::ostream;
using std::logic_error;
using std::invalid_argument;
using ifmo_mtp_utils::rnd;
using ifmo_mtp_utils::require;
typedef int WeightType;
#define WEIGHT_INF INT_MAX

struct Edge {
	int dest;
	WeightType w;
	int hlighted; // used to highlight the shortest paths
};

bool operator==(const Edge &e1, const Edge &e2) {
	return e1.dest == e2.dest && e1.w == e2.w;
}

const struct {
	bool operator()(const Edge &e1, const Edge &e2) const {
		return e1.dest < e2.dest;
	}
} edgeByDestComp;

// Tokens for very simplified DOT format
static struct {
	const char *metaNumNodes = "//nodes",
		*digraph = "digraph",
		*openGraphDef = "{",
		*lr = "rankdir=LR",
		*closeGraphDef = "}",
		*edge = "->",
		*openAttrsDef = "[",
		*closeAttrsDef = "]",
		*labelAttr = "label=",
		*highlight = "color=red",
		*comma = ", ",
		*space = " ",
		*tab = "\t",
		*newline = "\n";
} tokens;

/**
 * Generate connected graph with the required {@code density}.
 * Density is defined as {@code nEdges / maxnEdges}.
 * Min edges count is {@code 2*(nNodes - 1)} for connectivity
 */
vector<Edge>* generateGraph(int nNodes, double density, WeightType startWeight, WeightType endWeight) {
	vector<bool> nodes(nNodes);
	int nodesInSpanningTree = 0;
	vector<Edge>* adjLists = new vector<Edge>[nNodes];

	int u = int(rnd(int64_t(0), nNodes)), v = int(rnd(int64_t(0), nNodes));
	nodes[u] = nodes[v] = true;
	adjLists[u].push_back(Edge{v, 0, 0}); adjLists[v].push_back(Edge{u, 0, 0});
	nodesInSpanningTree += 2;

	while (nodesInSpanningTree < nNodes) {
		u = int(rnd(int64_t(0), nNodes)); v = int(rnd(int64_t(0), nNodes));
		if (nodes[u] ^ nodes[v]) {
			adjLists[u].push_back(Edge{v, 0, 0});
			adjLists[v].push_back(Edge{u, 0, 0});
			nodes[u] = nodes[v] = true;
			nodesInSpanningTree++;
		}
	}

	int edgesCount = int(nNodes*nNodes*density - 2*(nNodes - 1));
	for (int i = 0; i < edgesCount; i++) {
		u = int(rnd(int64_t(0), nNodes)); v = int(rnd(int64_t(0), nNodes));
		Edge e{v, 0, 0};
		if (std::find(adjLists[u].begin(), adjLists[u].end(), e) != adjLists[u].end()) {
			i--; continue;
		}
		adjLists[u].push_back(e);
	}

	for (u = 0; u < nNodes; u++) {
		int degree = int(adjLists[u].size());
		for (int j = 0; j < degree; j++)
			adjLists[u][j].w = int(rnd(int64_t(startWeight), endWeight));
		std::sort(adjLists[u].begin(), adjLists[u].end(), edgeByDestComp);
	}

	return adjLists;
}

static string getMsgUnableRead(const char* expectedToken) {
	return string("Unable to read '") + expectedToken + "'";
}
static string getMsgExpected(const string &token, const char* expectedToken) {
	return string("'") + expectedToken
		+ "' expected, given '" + token + "'";
}

vector<Edge>* parseDot(istream &stream, int &nNodes) {
	string token;
	require(stream >> token, getMsgUnableRead(tokens.metaNumNodes));
	require(token == tokens.metaNumNodes,
		getMsgExpected(token, tokens.metaNumNodes));

	require(stream >> nNodes, "Unable to read numNodes");
	require(nNodes > 0, "numNodes must be positive, given " + token);
	vector<Edge>* adjLists = new vector<Edge>[nNodes];

	require(stream >> token, getMsgUnableRead(tokens.digraph));
	require(token == tokens.digraph,
		getMsgExpected(token, tokens.digraph));

	require(stream >> token, getMsgUnableRead(tokens.openGraphDef));
	require(token == tokens.openGraphDef,
		getMsgExpected(token, tokens.openGraphDef));

	int labelAttrTokenLen = -1;
	while (tokens.labelAttr[++labelAttrTokenLen]);
	while (stream >> token && token != tokens.closeGraphDef) {
		int u = stoi(token);
		require(0 <= u && u < nNodes,
			"Source number must be in [0; numNodes), given " + token);

		require(stream >> token, getMsgUnableRead(tokens.edge));
		require(token == tokens.edge,
			getMsgExpected(token, tokens.edge));

		Edge e{0, 0, 0};
		require(stream >> e.dest, "Unable to read destination");
		require(0 <= e.dest && e.dest < nNodes,
			"Destination must be in [0; numNodes), given " + token);

		require(stream >> token, getMsgUnableRead(tokens.openAttrsDef));
		require(token == tokens.openAttrsDef,
			getMsgExpected(token, tokens.openAttrsDef));

		require(stream >> token, getMsgUnableRead(tokens.labelAttr));
		require(token.compare(0, labelAttrTokenLen, tokens.labelAttr) == 0,
			getMsgExpected(token, tokens.labelAttr));

		token = token.substr(labelAttrTokenLen);
		try {
			e.w = stoi(token);
		} catch(const logic_error &) {
			throw invalid_argument("Unable to parse weight '" + token + "'");
		}

		require(e.w >= 0, "Weight must be non-negative, given " + token);
		adjLists[u].push_back(e);

		while (stream >> token && token != tokens.closeAttrsDef);
		require(stream, getMsgExpected(token, tokens.closeAttrsDef));
	}
	require(stream, getMsgExpected(token, tokens.closeGraphDef));
	
	for (int i = 0; i < nNodes; i++)
		std::sort(adjLists[i].begin(), adjLists[i].end(), edgeByDestComp);

	return adjLists;
}

void writeDot(ostream &stream, vector<Edge>* adjLists, int &nNodes) {
	require(stream << tokens.metaNumNodes
		<< tokens.space << nNodes << tokens.newline, "Unable to write numNodes");
	require(stream << tokens.digraph << tokens.space
		<< tokens.openGraphDef << tokens.newline,
		string("Unable to write '") + tokens.digraph
		+ " " + tokens.openGraphDef + "'");
	require(stream << tokens.tab << tokens.lr << tokens.newline,
		string("Unable to write '") + tokens.lr + "'");

	for (int u = 0; u < nNodes; u++) {
		std::sort(adjLists[u].begin(), adjLists[u].end(), edgeByDestComp);
		int degree = int(adjLists[u].size());

		for (int j = 0; j < degree; j++) {
			const Edge &e = adjLists[u][j];
			require(stream << tokens.tab << u << tokens.space
				<< tokens.edge << tokens.space
				<< e.dest << tokens.space
				<< tokens.openAttrsDef << tokens.space
				<< tokens.labelAttr << e.w, "Unable to write edge");

			if (e.hlighted)
				require(stream << tokens.comma << tokens.highlight,
					"Unable to write highlight");

			require(stream << tokens.space << tokens.closeAttrsDef
				<< tokens.newline, "Unable to close edge def");
		}
	}

	require(stream << tokens.closeGraphDef, "Unable to close graph def");
}

vector<Edge>* copy(vector<Edge>* adjLists, int nNodes) {
	vector<Edge>* adjListsCopy = new vector<Edge>[nNodes];
	for (int u = 0; u < nNodes; u++) {
		int degree = int(adjLists[u].size());
		adjListsCopy[u].resize(degree);
		std::copy(adjLists[u].begin(), adjLists[u].end(), adjListsCopy[u].begin());
	}
	return adjListsCopy;
}
}