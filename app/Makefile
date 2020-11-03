all: application

INCLUDE=-I/usr/include/mysql
LIBS=-lncurses -lgdbm_compat -lgdbm
MYSQL=-lmysqlclient -L/usr/lib64/mysql


CFLAGS=

view.o: view.c
	gcc $(CFLAGS) -c view.c

access.o: access.c 
	gcc $(CFLAGS) -c access.c $(INCLUDE)

main.o: main.c
	gcc $(CFLAGS) -c main.c

application: view.o access.o main.o
	gcc $(CFLAGS) -o application view.o access.o main.o $(LIBS) $(MYSQL)

clean:
	rm -f application *.o

rmdb:
	rm -f *.pag *.dir
