PROJECT=c202
TESTS=$(PROJECT)-test $(PROJECT)-test-advanced
TEST_OUTPUTS=$(PROJECT)-test.out $(PROJECT)-test-advanced.out
PROG=$(PROJECT)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
SHELL=bash

.PHONY: all run tests clean purge

all: $(PROG)

run: $(PROG)
	@./$(PROG) > current-test.out
	@echo -e "\nTest output differences:"
	@diff -u --report-identical-files --text $(PROG).out current-test.out
	@rm -f current-test.out

$(PROJECT)-test: $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c

$(PROJECT)-test.out: $(PROJECT)-test
	@if [ -f $(PROJECT)-test.out ]; then echo "The reference output already exists!"; exit 1; fi
	./$(PROJECT)-test > $(PROJECT)-test.out

$(PROJECT)-test-advanced: $(PROJECT).c $(PROJECT)-test-advanced.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-test-advanced.c $(PROJECT)-test-utils.c

$(PROJECT)-test-advanced.out: $(PROJECT)-test-advanced
	@if [ -f $(PROJECT)-test-advanced.out ]; then echo "The reference output already exists!"; exit 1; fi
	./$(PROJECT)-test-advanced > $(PROJECT)-test-advanced.out

reference: $(TEST_OUTPUTS)

zadani: $(PROJECT)-test.out
	@mkdir $@ 2>/dev/null || echo "Directory exists, continuing"
	@unifdef -DSTUDENT_BUILD=1 -o $@/$(PROJECT).c $(PROJECT).c || printf ""
	@cp $(PROJECT).h $@/
	@cp $(PROG).c $@/
	@cp $(PROJECT)-test-utils.* $@/
	@cp $(PROJECT)-test.out $@/
	@cp Makefile $@/
	@tar -czvf $(PROJECT)-`date +%Y-%m-%d-%H-%M`.tar.gz $@/*

reseni:
	@mkdir $@

clean:
	@rm -f *.o $(PROG) $(TESTS)
	@rm -rf zadani reseni

purge: clean
	@rm -Rf $(PROJECT)-output
	@rm -f *.out *.txt *.html
