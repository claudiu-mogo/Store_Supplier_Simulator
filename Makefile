FLAG = -g -Wall -lm -std=c99
 
build: tema3

tema3:
	gcc -o tema3 main.c heap.c stack.c lists.c queue.c $(FLAG)

# Regulile de clean 
.PHONY : clean
clean :
	rm -f tema3
	rm -f *.out