#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "include/view.h"
#include "include/access.h"

int main(int argc, char **argv)
{

	int choice = 0;
	int ret; 
	initscr();

	ret = db_initialize(0);
	if(ret == 1){
		//error_msg_ui("database failed to initialize.");
		db_close();
		exit(1);
	}

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

	db_close();
	endwin();
	exit(EXIT_SUCCESS);
}
