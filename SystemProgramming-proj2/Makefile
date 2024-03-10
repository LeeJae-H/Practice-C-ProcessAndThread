all: proj2
	
proj2:	proj2.o main.o
	gcc -o proj2 proj2.o main.o -lpthread

proj2.o: proj2.c
	gcc -c proj2.c

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o
	rm -f proj2
