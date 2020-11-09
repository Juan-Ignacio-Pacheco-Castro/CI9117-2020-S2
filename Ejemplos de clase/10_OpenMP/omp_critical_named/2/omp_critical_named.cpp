#include <iostream>
#include <string>
#include <omp.h>

static void printTreadId() {
	#pragma omp critical (C1)
	{
		std::cout << "\tblocked C1 critical " << "\n";
		std::cout << "Thread " << omp_get_thread_num() << " from C1\n";
		std::cout << "\tfreed C1 critical " << "\n";
	}
}

static void printTreadId2() {
	#pragma omp critical (C2)
	{
		std::cout << "\tblocked C2 critical " << "\n";
		std::cout << "Thread " << omp_get_thread_num() << " from C2\n";
		std::cout << "\tfreed C2 critical " << "\n";
	}
}

int main() {

	#pragma omp parallel for num_threads(10)
	for (int i = 1; i <= 10; ++i) {
		if (omp_get_thread_num() % 2) {
			printTreadId();
		} else {
			printTreadId2();
		}
	}

	return 0;
}
