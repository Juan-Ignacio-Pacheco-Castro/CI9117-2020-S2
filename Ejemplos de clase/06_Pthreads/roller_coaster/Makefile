CFLAGS=-g -Wall -Wextra -pthread

all: roller_coaster

roller_coaster: roller_coaster.c main.c
	g++ $(CFLAGS) $^ -o $@.out

clean:
	rm -rf roller_coaster.out
