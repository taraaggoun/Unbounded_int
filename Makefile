CC = gcc
CCFLAGS = -Wall -g -pedantic
TARGETS = test_unbounded calc_unbounded_int
all: $(TARGETS)

test_unbounded : test_unbounded.o unbounded_int.o
	$(CC) -o test_unbounded test_unbounded.o unbounded_int.o

calc_unbounded_int: calc_unbounded_int.o unbounded_int.o
	$(CC) -o calc_unbounded_int calc_unbounded_int.o unbounded_int.o

test_unbounded.o: test_unbounded.c unbounded_int.h
	$(CC) $(CCFLAGS) -c test_unbounded.c

unbounded_int.o : unbounded_int.c unbounded_int.h
	$(CC) $(CCFLAGS) -c unbounded_int.c 

calc_unbounded_int.o : calc_unbounded_int.c calc_unbounded_int.h
	$(CC) $(CCFLAGS) -c calc_unbounded_int.c

clean :
	rm -rf *.o
	rm -rf $(TARGETS)