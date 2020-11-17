#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <curses.h>

#include "view.h"
#include "access.h"

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

#define MSG_LINE 6 /*Misc. message on this line*/
#define ERROR_LINE 22 /*Line to use for errors*/
#define Q_LINE 20 /*Line for questions*/
#define PROMPT_LINE 18 /*Line for prompting on*/

static void draw_menu_color(char *choice[], int selected, int start_row, int start_col)
{
	char **options;
	options = choice;
	char *selected_option;
	int i = 1;

	init_pair(selected, COLOR_WHITE, COLOR_RED);

	while(*options){
		if(selected == i){
			attron(COLOR_PAIR(selected)|A_BOLD);
			mvprintw(start_row + selected, start_col, "%s", *options);
			attroff(COLOR_PAIR(selected)|A_BOLD);
			options++;
			i++;
			continue;
		}
		mvprintw(start_row + i, start_col, "%s", *options);
		options++;
		i++;
	}
	mvprintw(start_row + i + 3, start_col, "Move highlight then press Return ");
	refresh();

		
}

static void draw_menu(char *choice[], int selected, int start_row, int start_col)
{
	char **options;
	options = choice;
	char *selected_option;
	int i = 1;

	while(*options){
		if(selected == i){
			attron(A_BOLD|A_STANDOUT);
			mvprintw(start_row + selected, start_col, "%s", *options);
			attroff(A_BOLD|A_STANDOUT);
			options++;
			i++;
			continue;
		}
		mvprintw(start_row + i, start_col, "%s", *options);
		options++;
		i++;
	}

	mvprintw(start_row + i + 3, start_col, "Move highlight then press Return ");
	refresh();
		
}

static int get_confirm()
{
	char in;
	int confirmed = 0;
	mvprintw(Q_LINE, 10, "Are you sure? ");
	clrtoeol();
	refresh();

	cbreak();
	in = getch();
	if(in == 'y' || in == 'Y'){
		confirmed = 1;
	}

	nocbreak();

	if(!confirmed){
		mvprintw(Q_LINE, 10, "Cancelled");
		clrtoeol();
		refresh();
		sleep(1);
	}

	return confirmed;
}


static void clear_all_screen()
{
	clear();
	mvprintw(2, 20, "CD Database Application");
	if(get_current_cd0()){
		mvprintw(ERROR_LINE, 0, "Current Record: %s\n",get_current_cd());
	}
	refresh();
}

/*curse color mode*/
static void start_color_mode()
{
	
	if(!has_colors()){
		endwin();
		fprintf(stderr, "Error - no color support on this terminal\n");
		exit(1);
	}

	if(start_color() != OK){
		endwin();
		fprintf(stderr, "Error - could not initialize colors\n");
		exit(1);
	}

}

void get_return()
{
	int ch;
	mvprintw(ERROR_LINE + 1, 0, "Press return ");
	refresh();
	while((ch = getchar()) != '\n' && ch != EOF);
}

int getchoice()
{
	
	int key = 0;
	int selected = 1;
	int option_cnt = 0;
	int start_row = 5, start_col = 10;
	char **options;
	char **choice;

	choice = get_current_cd0() ? extend_menu : main_menu;
	options = choice;

	while(*options){
		options++;
		option_cnt++;
	}
	
	clear_all_screen();

	noecho();
	cbreak();

	mvprintw(start_row - 2,start_col,"Options: ");
	
	keypad(stdscr, TRUE);

	while(key != ERR && key != 10 ){

		if( key == KEY_DOWN){
			if(selected == option_cnt)
				selected = 1;
			else
				selected++;
		}

		if( key == KEY_UP){
			if(selected == 1)
				selected = option_cnt;
			else 
				selected--;
		}
		
		draw_menu(choice, selected, start_row, start_col);
		key = getch();
	}

	keypad(stdscr,FALSE);
	echo();
	nocbreak();

	return choice[selected-1][0]; 
}

void add_record_ui()
{
	char title[MAX_LEN] = {0};
	char artist[MAX_LEN] = {0};
	char cd_entry[MAX_LEN] = {0};

	int start_row = MSG_LINE, start_col = 10;
	int ret = 0;

	clear();

	mvprintw(start_row - 2, start_col,"%s","Enter new record details:");

	mvprintw(start_row, start_col, "%s","Record Title:");
	getstr(title);

	mvprintw(start_row + 1, start_col, "%s","Record Artist:");
	getstr(artist);


	mvprintw(PROMPT_LINE - 2, start_col, "About to add this new entry:");
	sprintf(cd_entry, "%s, %s",title, artist);
	mvprintw(PROMPT_LINE, 10, "%s", cd_entry);

	refresh();

	if(get_confirm()){
		ret=add_record_wrap(title,artist);
		if(ret == 0){
			fulfill_current_cd(title);
		}else{
			mvprintw(Q_LINE,start_col, "%s exsits.",title);
			clrtoeol(); /*delete chars behind 'exsits'*/
			refresh();
			sleep(1);
		}
	}
	
}

void remove_record_ui()
{
	
	int ret = 0;
	char title[MAX_LEN] = {0};

	mvprintw(PROMPT_LINE, 0, "About to remove Record: %s", get_current_cd());

	if(!get_confirm())
		return;

	remove_all_tracks_of_one_record(get_current_cd());

	ret = remove_record_wrap(get_current_cd());
	if(ret == 1){ 
		mvprintw(ERROR_LINE, 0, "Failed");
		clrtoeol();
		refresh();
		sleep(1);
	}
	zero_current_cd();
	refresh();
}

void find_record_ui() 
{ 
	char title[MAX_LEN] = {0};

	mvprintw(Q_LINE, 0,"%s","Enter the title to search for:");
	getstr(title);

	refresh();

	if(record_exsits(title)){
		fulfill_current_cd(title);
	} else{
		mvprintw(ERROR_LINE, 0, "Sorry, no matching record found. ", title);
		get_return();
	}

	refresh();
}

static void list_track_ui_ver1(int start_row, int start_col)
{
		
	clear_all_screen();
	mvprintw(start_row - 2, start_col,"%s","Track list of current Record:");
	list_track_by_title_of_record(start_row, start_col, get_current_cd()); 
	get_return();
}


static void list_track_ui_ver2(int start_row, int start_col)
{

	int try = 3;
	int ret = 0;
	char title[MAX_LEN] = {0};

	while(try){
		clear_all_screen();
		mvprintw(start_row - 2, start_col,"%s","Track list of seraching Record:");
		mvprintw(start_row, start_col, "%s","Record Title:");
		getstr(title);
	
		if((ret = list_track_by_title_of_record(start_row + 1, start_col, title)) == 0){
			break;
		}

		if(try > 1)
			mvprintw(start_row + 3, start_col, "%d times left", try - 1);

		refresh();

		try--;

		sleep(2);

	}

	get_return();	
}

void list_track_ui()
{
	int start_row = 8, start_col = 10;
	if(get_current_cd0()){
		list_track_ui_ver1(start_row, start_col);
	}else{
		list_track_ui_ver2(start_row, start_col);
	}
	
}

void display_all_records_ui()
{
	int start_row = 5, start_col = 10;
	clear_all_screen();
	display_all_records(start_row, start_col);
	get_return();
	
}


/*
  Some defines we use only for showing or entering the track information
 */
#define BOXED_ROWS    11
#define BOXED_COLS    60
#define BOX_ROW_POS   8
#define BOX_COL_POS    2

int add_track_ui()
{

	WINDOW *box_win_ptr;
	WINDOW *sub_win_ptr;
	char title[MAX_LEN];
	int screen_row = 1;
	int track = 1;


	mvprintw(PROMPT_LINE, 0, "Re-entering tracks for record. ");
	if(!get_confirm())
		return 1;

	/*Firstly,delete all the exsiting tracks*/
	remove_all_tracks_of_one_record(get_current_cd());

	clear_all_screen();
	mvprintw(MSG_LINE,0,"Enter a blank line to finish");
	refresh();

	box_win_ptr = subwin(stdscr, BOXED_ROWS + 2, BOXED_COLS + 2, BOX_ROW_POS - 1, BOX_COL_POS - 1);
	if(!box_win_ptr)
		return 1;
	box(box_win_ptr, ACS_VLINE, ACS_HLINE);
	touchwin(box_win_ptr);
	wrefresh(box_win_ptr);

	sub_win_ptr = subwin(stdscr, BOXED_ROWS, BOXED_COLS, BOX_ROW_POS, BOX_COL_POS); 
	if(!sub_win_ptr)
		return 1;

	scrollok(sub_win_ptr,TRUE);

	do{
		mvwprintw(sub_win_ptr, screen_row++, BOX_COL_POS + 2, "Track %d: ",track++);	
		clrtoeol();
		wgetnstr(sub_win_ptr, title, MAX_LEN);

		if(*title){
			int ret = add_track_wrap(title, "jazz",track - 1);
			if(ret == 1 ){
				mvprintw(ERROR_LINE, 0, "Failed");
				clrtoeol();
				refresh();
				get_return();
			}
		}

		if(screen_row > BOXED_ROWS-2){
			scroll(sub_win_ptr);
			screen_row--;
		}
	
	}while(*title);

}
