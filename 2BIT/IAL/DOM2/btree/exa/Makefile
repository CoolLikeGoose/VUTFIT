CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES_REC=exa.c ../rec/btree.c ../btree.c ../test_util.c ../test.c
FILES_ITER=exa.c ../iter/btree.c ../iter/stack.c ../btree.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_rec $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_iter $(FILES_ITER)

clean:
	rm -f test_rec
	rm -f test_iter
