/* *** */

int main(int argc, char** argv) {
#define DEFAULT_BLOCK {\
	int matrix_product_main(int argc, char **argv);\
	return matrix_product_main(argc, argv);\
}

#if defined LAB_NUM
	#if LAB_NUM == 1
		DEFAULT_BLOCK;
	#elif LAB_NUM == 2
		int jacobi_main(int argc, char **argv);
		return jacobi_main(argc, argv);
	#elif LAB_NUM == 3
		int qsort_main(int argc, char **argv);
		return qsort_main(argc, argv);
	#elif LAB_NUM == 4
		int dijkstra_main(int argc, char **argv);
		return dijkstra_main(argc, argv);
	#else
		#error Unknown work
	#endif
#else
	DEFAULT_BLOCK;
#endif
}
