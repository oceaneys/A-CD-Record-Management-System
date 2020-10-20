all: application

INCLUDE=
LIBS=-lncurses

CFLAGS=

view.o: view.c main.o
	gcc $(CFLAGS) -c view.c

methods.o: methods.c 
	gcc $(CFLAGS) -c methods.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

application: view.o methods.o main.o
	gcc $(CFLAGS) -o application view.o methods.o main.o $(LIBS)

clean:
	rm -f application *.o
