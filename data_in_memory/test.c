#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "data.h"

char *menu[] = {
	"a - add new record",
	"d - delete a record",
	"f - find a record",
	"q - quit",
	NULL,
};

int main(int argc, char **argv)
{

	int choice = 0;
	initscr();

	start_color_mode();

	do{
		choice = getchoice("Options:", menu);
		switch(choice){
			case 'q':
				break;
			case 'a':
				mvprintw(20,10,"add new record");
				break;
			case 'd':
				mvprintw(20,10,"delete a record");
				break;
			case 'f':
				mvprintw(20,10,"find a record");
				break;
				
			}
		}while(choice != 'q');

	endwin();
	exit(EXIT_SUCCESS);


}
