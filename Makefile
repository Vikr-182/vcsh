
Shell:	main.o
		touch main.c
		gcc  -g -w -o  Shell main.o -lncurses

main.o: main.c 
		gcc -g -w -c main.c

clean:
		rm *.o
