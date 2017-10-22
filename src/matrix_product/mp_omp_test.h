#pragma once
#include <iostream>
#include <ctime>
#include "mp_omp_defs.h"

namespace ifmo_mtp_omp {
#define TEST_SIZES_COUNT 27
#define TEST_SIZES {\
			{ 1, 5000000, 1 },\
			{ 1, 10000000, 1 },\
			{ 1, 50000000, 1 },\
			{ 1, 5000, 1000 },\
			{ 1, 5000, 5000 },\
			{ 1, 5000, 10000 },\
			{ 1, 10000, 1000 },\
			{ 1, 10000, 5000 },\
			{ 1, 10000, 10000 },\
			{ 1000, 5000, 1 },\
			{ 5000, 5000, 1 },\
			{ 10000, 5000, 1 },\
			{ 1000, 10000, 1 },\
			{ 5000, 10000, 1 },\
			{ 10000, 10000, 1 },\
			{ 100, 1000, 100 },\
			{ 200, 1000, 200 },\
			{ 300, 1000, 300 },\
			{ 50, 5000, 50 },\
			{ 100, 5000, 100 },\
			{ 200, 5000, 200 },\
			{ 200, 100, 200 },\
			{ 500, 100, 500 },\
			{ 1000, 100, 1000 },\
			{ 2000, 1, 2000 },\
			{ 2500, 1, 2500 },\
			{ 3000, 1, 3000 },\
}

#define LABELS {\
			"TITLE",\
			"SEQ",\
			"STATIC",\
			"DEEP_STATIC",\
			"DDEEP_STATIC",\
			"STATIC1",\
			"DEEP_STATIC1",\
			"DDEEP_STATIC1",\
			"STATIC10",\
			"DEEP_STATIC10",\
			"DDEEP_STATIC10",\
			"STATIC100",\
			"DEEP_STATIC100",\
			"DDEEP_STATIC100",\
			"DYNAMIC1",\
			"DEEP_DYNAMIC1",\
			"DDEEP_DYNAMIC1",\
			"DYNAMIC10",\
			"DEEP_DYNAMIC10",\
			"DDEEP_DYNAMIC10",\
			"DYNAMIC100",\
			"DEEP_DYNAMIC100",\
			"DDEEP_DYNAMIC100",\
			"GUIDED1",\
			"DEEP_GUIDED1",\
			"DDEEP_GUIDED1",\
			"GUIDED10",\
			"DEEP_GUIDED10",\
			"DDEEP_GUIDED10",\
			"GUIDED100",\
			"DEEP_GUIDED100",\
			"DDEEP_GUIDED100",\
			"MANUAL"\
}

clock_t* getMulTime(Matrix<> &a, Matrix<> &b) {
	static clock_t results[NUM_IMPLS];

	clock_t startTime = clock();
	mp(a, b);
	results[0] = clock() - startTime;

	startTime = clock();
	mp2(a, b);
	results[1] = clock() - startTime;

	startTime = clock();
	mp3(a, b);
	results[2] = clock() - startTime;

	startTime = clock();
	mp4(a, b);
	results[3] = clock() - startTime;

	startTime = clock();
	mp5(a, b);
	results[4] = clock() - startTime;

	startTime = clock();
	mp6(a, b);
	results[5] = clock() - startTime;

	startTime = clock();
	mp7(a, b);
	results[6] = clock() - startTime;

	startTime = clock();
	mp8(a, b);
	results[7] = clock() - startTime;

	startTime = clock();
	mp9(a, b);
	results[8] = clock() - startTime;

	startTime = clock();
	mp10(a, b);
	results[9] = clock() - startTime;

	startTime = clock();
	mp11(a, b);
	results[10] = clock() - startTime;

	startTime = clock();
	mp12(a, b);
	results[11] = clock() - startTime;

	startTime = clock();
	mp13(a, b);
	results[12] = clock() - startTime;

	startTime = clock();
	mp14(a, b);
	results[13] = clock() - startTime;

	startTime = clock();
	mp15(a, b);
	results[14] = clock() - startTime;

	startTime = clock();
	mp16(a, b);
	results[15] = clock() - startTime;

	startTime = clock();
	mp17(a, b);
	results[16] = clock() - startTime;

	startTime = clock();
	mp18(a, b);
	results[17] = clock() - startTime;

	startTime = clock();
	mp19(a, b);
	results[18] = clock() - startTime;

	startTime = clock();
	mp20(a, b);
	results[19] = clock() - startTime;

	startTime = clock();
	mp21(a, b);
	results[20] = clock() - startTime;

	startTime = clock();
	mp22(a, b);
	results[21] = clock() - startTime;

	startTime = clock();
	mp23(a, b);
	results[22] = clock() - startTime;

	startTime = clock();
	mp24(a, b);
	results[23] = clock() - startTime;

	startTime = clock();
	mp25(a, b);
	results[24] = clock() - startTime;

	startTime = clock();
	mp26(a, b);
	results[25] = clock() - startTime;

	startTime = clock();
	mp27(a, b);
	results[26] = clock() - startTime;

	startTime = clock();
	mp28(a, b);
	results[27] = clock() - startTime;

	startTime = clock();
	mp29(a, b);
	results[28] = clock() - startTime;

	startTime = clock();
	mp30(a, b);
	results[29] = clock() - startTime;

	startTime = clock();
	mp31(a, b);
	results[30] = clock() - startTime;

	startTime = clock();
	mpManual(a, b);
	results[31] = clock() - startTime;

	return results;
}

bool testMpEquality(Matrix<> &a, Matrix<> &b)
{
	std::equal_to<Matrix<>> equalTo = std::equal_to<Matrix<>>();
	Matrix2D<> c1 = mp(a, b), c2 = mp2(a, b), c3 = mp3(a, b), c4 = mp4(a, b),
	c5 = mp5(a, b), c6 = mp6(a, b), c7 = mp7(a, b), c8 = mp14(a, b), c9 = mp15(a, b),
	c10 = mp16(a, b), c11 = mp23(a, b), c12 = mp24(a, b), c13 = mp25(a, b), c14 = mpManual(a, b);
	return equalTo(c1, c2) && equalTo(c1, c3) && equalTo(c1, c4) && equalTo(c1, c5) &&
		   equalTo(c1, c6) && equalTo(c1, c7) && equalTo(c1, c8) && equalTo(c1, c9) &&
		   equalTo(c1, c10) && equalTo(c1, c11) && equalTo(c1, c12) && equalTo(c1, c13) &&
		   equalTo(c1, c14);
}

bool testMpResult() {
	const int nAttempts = 10;
	const double leftBnd = -1000, rightBnd = 1000;
	const size_t precision = 2;
	for (int i = 0; i < nAttempts; i++) {
		Matrix2D<> a = ifmo_mtp_utils::rndMatrix(1, 1, leftBnd, rightBnd, precision);
		Matrix2D<> b = ifmo_mtp_utils::rndMatrix(1, 1, leftBnd, rightBnd, precision);
		if (!testMpEquality(a, b)) {
			return false;
		}

		a.slf = ifmo_mtp_utils::rndMatrix(1, 1000, leftBnd, rightBnd, precision);
		b.slf = ifmo_mtp_utils::rndMatrix(1000, 1, leftBnd, rightBnd, precision);
		if (!testMpEquality(a, b)) {
			return false;
		}

		a.slf = ifmo_mtp_utils::rndMatrix(1, 1000, leftBnd, rightBnd, precision);
		b.slf = ifmo_mtp_utils::rndMatrix(1000, 100, leftBnd, rightBnd, precision);
		if (!testMpEquality(a, b)) {
			return false;
		}

		a.slf = ifmo_mtp_utils::rndMatrix(100, 1000, leftBnd, rightBnd, precision);
		b.slf = ifmo_mtp_utils::rndMatrix(1000, 1, leftBnd, rightBnd, precision);
		if (!testMpEquality(a, b)) {
			return false;
		}

		a.slf = ifmo_mtp_utils::rndMatrix(100, 1000, leftBnd, rightBnd, precision);
		b.slf = ifmo_mtp_utils::rndMatrix(1000, 100, leftBnd, rightBnd, precision);
		if (!testMpEquality(a, b)) {
			return false;
		}
	}
	return true;
}
}
