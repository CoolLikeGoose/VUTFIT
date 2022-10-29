define NEW_LINE
endef
compile: t9search.c
	gcc -std=c99 -Wall -Wextra -Werror t9search.c -o t9search
	t9search.exe 1234 <seznam.txt
tests: t9search.exe
	@echo 1.
	@echo ---Testing for number searching---
	t9search.exe 1234 <seznam.txt

	@echo 2.
	@echo ---Testing for multiple number searching---
	t9search.exe 54 <seznam.txt

	@echo 3.
	@echo ---Testing for name searching---
	t9search.exe 686 <seznam.txt

	@echo 4.
	@echo ---Testing for multiple name searching---
	t9search.exe 62 <seznam.txt

	@echo 5.
	@echo ---Testing for Not Found---
	t9search.exe 645454654546 <seznam.txt

	@echo 6.
	@echo ---Testing for Found Everything---
	t9search.exe <seznam.txt
