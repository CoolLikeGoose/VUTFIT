CC=gcc
CFLAGS=-g -Wall -std=c99
FILES= functions.c functions.h main.c test.c makefile


.PHONY: all runtest run pack clean buildall cleanall

all: functions.c functions.h main.c
	$(CC) $(CFLAGS) ./functions.c ./main.c -o main

test: functions.c functions.h test.c
	$(CC) $(CFLAGS) ./functions.c ./test.c -o tests

runtest: tests
	./tests

pack: functions.c functions.h main.c test.c makefile
	zip program.zip $(FILES)
