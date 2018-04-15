CFLAGS = -Wall -std=c99

all: linesort linesort_tree

linesort: linesort.c
	gcc $(CFLAGS) -o $@ linesort.c

linesort_tree: linesort_tree.c
	gcc $(CFLAGS) -o $@ linesort_tree.c

test: test.sh all
	./test.sh

clean:
	rm *.txt* linesort linesort_tree testing
