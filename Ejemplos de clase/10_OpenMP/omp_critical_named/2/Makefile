CFLAGS=-g -Wall -Wextra -fopenmp

all: omp_critical_named

omp_critical_named: omp_critical_named.cpp
	g++ $(CFLAGS) $^ -lm -o $@.out

clean:
	rm -rf omp_critical_named.out
