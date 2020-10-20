#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "include/view.h"

int main(int argc, char **argv)
{

	int choice = 0;
	initscr();

	do{
		choice = getchoice();
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
			case 'u':
					add_track_ui();
					break;
				
			}
		}while(choice != 'q');

	endwin();
	exit(EXIT_SUCCESS);
}
