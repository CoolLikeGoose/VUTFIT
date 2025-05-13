CC=gcc
CFLAGS=--std=c99

main: main.o add_conditions.o
	$(CC) $(CFLAGS) -o main main.o add_conditions.o

main.o: main.c cnf.h
	$(CC) $(CFLAGS) -c main.c

add_conditions.o: add_conditions.c cnf.h
	$(CC) $(CFLAGS) -c add_conditions.c

clean:
	rm -f add_conditions.o main.o main

test:
	@python3 ../tests/run_tests.py
