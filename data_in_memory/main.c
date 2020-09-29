#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>


char *menu[] = {
	"a - add new record",
	"r - remove a record",
	"f - find a record",
	"l - list the tracks of a record",
	"d - display all the records",
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
					add_record_ui();
				  	break;
			case 'r':
					remove_record_ui();
					break;
			case 'f':
					find_record_ui();
					break;
			case 'l':
					list_track_ui();
					break;
			case 'd':
					display_all_records_ui();
					break;
				
			}
		}while(choice != 'q');

	endwin();
	exit(EXIT_SUCCESS);


}
