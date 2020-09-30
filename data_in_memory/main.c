#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "include/data.h"

char *main_menu[] = {
	"add new record",
	"find a record",
	"display all the records",
	"quit",
	NULL,

};

char *extend_menu[] = {
	"add new record",
	"find a record",
	"display all the records",
	"remove current record",
	"list tracks on current record",
	"update track information on current record",
	"quit",
	NULL,
};

int main(int argc, char **argv)
{

	int choice = 0;
	initscr();

	//start_color_mode();

	do{
		choice = getchoice("Options:",current_cd[0] ? extend_menu : main_menu);
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
