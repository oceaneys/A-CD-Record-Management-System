all: application

INCLUDE=
LIBS=-lncurses

CFLAGS=

methods.o: methods.c 
	gcc $(CFLAGS) -c methods.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

application: methods.o main.o
	gcc $(CFLAGS) -o application methods.o main.o $(LIBS)

clean:
	rm -f application *.o
