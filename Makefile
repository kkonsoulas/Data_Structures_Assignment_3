
compile: tree1 tree2 htable1

CFLAGS = -Wall -g -std=c99

tree.o: tree.c tree.h
	gcc $(CFLAGS) tree.c -c -o tree.o

htable.o: htable.c htable.h
	gcc $(CFLAGS) htable.c -c
	
hbtree.o: hbtree.c hbtree.h
	gcc $(CFLAGS) hbtree.c -c
	
tree1:  hbtree.o tree1.c hbtree.h
	gcc $(CFLAGS) tree1.c hbtree.o -o tree1 -lm
	
tree2:  tree.o tree2.c
	gcc $(CFLAGS) tree2.c tree.o -o tree2 -lm
	
htable1: htable.o htable1.c
	gcc $(CFLAGS) htable1.c htable.o -o htable1

run: run_tree1 run_tree2 run_htable1

run_tree1:
	./tree1 < tests/tree1_a.in > tests/tree1_a.out
	diff -urN tests/tree1_a.std tests/tree1_a.out
	./tree1 < tests/tree1_b.in > tests/tree1_b.out
	diff -urN tests/tree1_b.std tests/tree1_b.out
	./tree1 < tests/tree1_c.in > tests/tree1_c.out
	diff -urN tests/tree1_c.std tests/tree1_c.out
	./tree1 < tests/tree1_d.in > tests/tree1_d.out
	diff -urN tests/tree1_d.std tests/tree1_d.out
	./tree1 < tests/tree1_e.in > tests/tree1_e.out
	diff -urN tests/tree1_e.std tests/tree1_e.out

run_tree2:
	./tree2 < tests/tree2_a.in > tests/tree2_a.out
	python3 check_tree2.py tests/tree2_a.in tests/tree2_a.std tests/tree2_a.out
	./tree2 < tests/tree2_b.in > tests/tree2_b.out
	python3 check_tree2.py tests/tree2_b.in tests/tree2_b.std tests/tree2_b.out
	./tree2 < tests/tree2_c.in > tests/tree2_c.out
	python3 check_tree2.py tests/tree2_c.in tests/tree2_c.std tests/tree2_c.out
	./tree2 < tests/tree2_d.in > tests/tree2_d.out
	python3 check_tree2.py tests/tree2_d.in tests/tree2_d.std tests/tree2_d.out
	./tree2 < tests/tree2_e.in > tests/tree2_e.out
	python3 check_tree2.py tests/tree2_e.in tests/tree2_e.std tests/tree2_e.out

run_htable1:
	./htable1 < tests/htable1_a.in > tests/htable1_a.out
	diff -urN tests/htable1_a.std tests/htable1_a.out
	./htable1 < tests/htable1_b.in > tests/htable1_b.out
	diff -urN tests/htable1_b.std tests/htable1_b.out
	./htable1 < tests/htable1_c.in > tests/htable1_c.out
	diff -urN tests/htable1_c.std tests/htable1_c.out
	./htable1 < tests/htable1_d.in > tests/htable1_d.out
	diff -urN tests/htable1_d.std tests/htable1_d.out
	./htable1 < tests/htable1_e.in > tests/htable1_e.out
	diff -urN tests/htable1_e.std tests/htable1_e.out
	
clean:
	-rm *.o tree1 tree2 htable1
	-rm tests/*.out

