all: application

INCLUDE=-I/usr/include/mysql
#LIBS=-lncurses -lgdbm_compat -lgdbm
LIBS=-lncurses 
MYSQL=-lmysqlclient -L/usr/lib64/mysql


CFLAGS=
view.o: view.c view.h access.h
	gcc $(CFLAGS) -c view.c

access.o: access.c access.h
	gcc $(CFLAGS) -c access.c  $(INCLUDE)

main.o: main.c view.h access.h
	gcc $(CFLAGS) -c main.c

application: view.o access.o main.o
	gcc $(CFLAGS) -o application view.o access.o main.o $(LIBS) $(MYSQL)

clean:
	rm -f application *.o

rmdb:
	rm -f *.pag *.dir
