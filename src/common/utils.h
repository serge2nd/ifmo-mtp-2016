#pragma once
#include <cmath>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "defs.h"

namespace ifmo_mtp_utils {
using std::vector;
using std::pair;
using std::string;
using std::to_string;
using std::ios;
using std::fstream;
using std::runtime_error;
using std::logic_error;
using std::invalid_argument;
using std::out_of_range;
#define argmin(a, i, j) (less((a)[i],(a)[j]) ? (i) : (j))
#define argmax(a, i, j) (less((a)[i],(a)[j]) ? (j) : (i))

/**
 * Random real in [start; end)
 * Uses {@code rand()}
 */
double rnd(double start, double end, double precision) {
	double x = start + (end - start)*rand()/RAND_LIM;
	return (precision == 0) ? x : (floor(x*precision)/precision);
}

/**
 * Random integer in [start; end)
 * Uses {@code rand()}
 */
int64_t rnd(int64_t start, int64_t end) {
	return int64_t(start + floor((end - start)*rand()/RAND_LIM));
}

/**
 * Uses {@code rand()}
 */
int rndSign() {
	return 2*int(floor(2*rand()/RAND_LIM)) - 1;
}

/**
 * @throws std::invalid_argument
 */
void require(bool condition, const string &msg) {
	if (!condition)
		throw invalid_argument(msg);
}

/**
 * @throws std::runtime_error
 */
void require(ios &stream, const string &msg) {
	if (!stream)
		throw runtime_error(msg);
}

/**
 * Checks if {@code i} is in range [start; end)
 * @throws std::out_of_range
 */
void requireRange(int64_t i, int64_t end, int64_t start) {
	if (i < start || i >= end)
		throw out_of_range(to_string(i) + " is out of range ["
				+ to_string(start) + ", " + to_string(end) + ")");
}

/**
 * Gets block size in {@code total} elements split into {@code n} blocks
 * by formula {@code (total + (total%n==0||total<n) ? 0 : (n - total%n)) / n}
 */
int64_t splitSize(int64_t total, int64_t n) {
	int64_t rest = (total < n) ? 0 : (total % n);
	int64_t compl_ = (rest == 0) ? 0 : (n - rest);

	int64_t part = (total + compl_) / n;
	if (part == 0) part = total;
	return part;
}

/**
 * @return {@code static_cast<T*>(operator new[](n * sizeof(T)));}
 */
template<class T> T* newArray(size_t n) {
	return static_cast<T*>(operator new[](n * sizeof(T)));
}

/**
 * Checks if {@code fileStream.is_open()}
 * @throws std::invalid_argument
 */
void checkStream(const char* filename, const fstream &fileStream) {
	if (!fileStream.is_open())
		throw invalid_argument(string("Cannot open file ") + filename);
}

/**
 * Iterative quick sort with pivot as median of random three elements.
 * Uses {@code rand()}
 */
template<class T>
void qsort(T* arr, size_t start, size_t end, const BinPred<T> &less = std::less<T>()) {
	if (start + 2 > end)
		return;
	vector<pair<size_t, size_t>> stack;
	pair<size_t, size_t> bounds(start, end);
	stack.push_back(bounds);

	while (!stack.empty()) {
		bounds = stack.back(); stack.pop_back();
		start = bounds.first; end = bounds.second;

		size_t p = argRndMedian3(arr, start, end, less);
		if (p != end - 1)
			std::swap(arr[end - 1], arr[p]);
		p = partition(arr, start, end, arr[end - 1], less);

		if (p != end - 1)
			std::swap(arr[end - 1], arr[p]);
		if (start + 2 <= p) {
			bounds.first = start; bounds.second = p;
			stack.push_back(bounds);
		}
		if (p + 3 <= end) {
			bounds.first = p + 1; bounds.second = end;
			stack.push_back(bounds);
		}
	}
}

/**
 * Partition procedure of quick sort algorithm
 */
template<class T>
size_t partition(T* arr, size_t start, size_t end, const T &pivot, const BinPred<T> &less = std::less<T>()) {
	if (end <= start)
		return 0;

	size_t i = start;
	for (size_t j = start; j < end; j++) {
		if (less(arr[j], pivot)) {
			if (i != j)
				std::swap(arr[i], arr[j]);
			i++;
		}
	}

	return i;
}

/**
 * Uses {@code rand()}
 */
template<class T>
size_t argRndMedian3(T* arr, size_t start, size_t end, const BinPred<T> &less = std::less<T>()) {
	size_t len = end - start,
		i1 = start + floor(len*rand()/RAND_LIM),
		i2 = start + floor(len*rand()/RAND_LIM),
		i3 = start + floor(len*rand()/RAND_LIM);
	size_t k = argmax(arr, i1, i2);
	return argmax(arr,
		argmin(arr, k, i3), argmin(arr, i1, i2));
}

/**
 * Unsigned bitwise right shift
 */
int rshift(int i, int n) {
	return (i >> n) & ~(UINT_MAX << (sizeof(int) * 8 - n));
}

/**
 * Efficiently gets the number of set bits (with Hamming distance)
 */
int getSetBitsNum(int i)
{
	i = i - (rshift(i, 1) & 0x55555555);
	i = (i & 0x33333333) + (rshift(i, 2) & 0x33333333);
	return rshift(((i + rshift(i, 4)) & 0x0F0F0F0F) * 0x01010101, 24);
}

/**
 * Picks up the min element among the root of binary tree and its childs to the root
 * and repeats this for the changed child as root if presented.
 * That restores binary heap if subtrees of the root were correct binary heaps
 * @param vals - values being moved to the same positions as keys
 * @param places - indexes of values in binary heap
 */
template<class T, class U>
void heapify(T* keys, size_t n, size_t i, U* vals = NULL, size_t* places = NULL, const BinPred<T> &less = std::less<T>()) {
	while (i < n/2) {
		size_t l = 2*i + 1, r = 2*i + 2,
				min = argmin(keys, i, l);
		if (r < n) min = argmin(keys, min, r);
		if (i == min)
			break;

		std::swap(keys[i], keys[min]);
		if (vals != NULL) {
			std::swap(vals[i], vals[min]);
		}
		if (places != NULL) {
			places[vals[i]] = i;
			places[vals[min]] = min;
		}

		i = min;
	}
}

/**
 * @param vals - values being moved to the same positions as keys
 * @param places - indexes of values in binary heap
 */
template<class T, class U>
void buildHeap(T* keys, size_t n, U* vals = NULL, size_t* places = NULL, const BinPred<T> &less = std::less<T>()) {
	for (size_t i = n/2 - 1; i >= 0; i--)
		heapify(keys, n, i, vals, places, less);
}

/**
 * Restores correct binary heap after decreasing the element at {@code i}
 * @param vals - values being moved to the same positions as keys
 * @param places - indexes of values in binary heap
 */
template<class T, class U>
void decreaseKey(T* keys, size_t n, size_t i, U* vals = NULL, size_t* places = NULL, const BinPred<T> &less = std::less<T>()) {
	if (i == 0 || i >= n)
		return;

	size_t parent = (i - 1)/2;
	while (i > 0 && less(keys[i], keys[parent])) {
		std::swap(keys[i], keys[parent]);
		if (vals != NULL) {
			std::swap(vals[i], vals[parent]);
		}
		if (places != NULL) {
			places[vals[i]] = i;
			places[vals[parent]] = parent;
		}
		i = parent; parent = (i - 1)/2;
	}
}

/**
 * Used to indicate that the method from which it's called is unsupported
 * @throws std::logic_error
 */
void unsupported() { throw logic_error("Unsupported operation"); }
}
