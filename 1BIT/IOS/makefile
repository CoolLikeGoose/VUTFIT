final: proj2.c
	gcc -std=gnu99 -Wall -Wextra -Werror -pedantic -lrt -pthread proj2.c -o proj2

test_run: proj2.c
	gcc -std=gnu99 -lrt -pthread proj2.c -o proj2
	./proj2 3 2 100 100 100
	cat proj2.out

test: proj2.c
	gcc -std=gnu99 -lrt -pthread proj2.c -o proj2
