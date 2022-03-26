run: a.out test
	./test | ./a.out
a.out: main.cpp
	g++ main.cpp
test: test.c
	gcc -o test test.c
