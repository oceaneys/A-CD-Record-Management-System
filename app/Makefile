all: application

INCLUDE=
LIBS=-lncurses -lgdbm_compat -lgdbm

CFLAGS=

view.o: view.c
	gcc $(CFLAGS) -c view.c

access.o: access.c 
	gcc $(CFLAGS) -c access.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

application: view.o access.o main.o
	gcc $(CFLAGS) -o application view.o access.o main.o $(LIBS)

clean:
	rm -f application *.o

rmdb:
	rm -f *.pag *.dir
