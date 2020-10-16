#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <curses.h>

#include "include/data.h"
#include "include/list.h"


#define MSG_LINE 6 /*Misc. message on this line*/
#define ERROR_LINE 22 /*Line to use for errors*/
#define Q_LINE 20 /*Line for questions*/
#define PROMPT_LINE 18 /*Line for prompting on*/

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};

/*two global list created for `find_*_by_title`*/
LIST_HEAD(record_result_list); 
LIST_HEAD(track_result_list);

/*Beginning of add record*/

int add_record(Record *record)
{

	record_list.record_cnt += 1; 
	list_add_tail(&record->list,&record_list.list);
	return 0;
} 


int add_record_wrap(char *title, char *artist, Record *record)
{

	if(get_record_by_title(title)){
		return 1;
	}

	*record = (struct Record){ 
	    .track=LIST_HEAD_INIT(record->track),
	    .list=LIST_HEAD_INIT(record->list)
	};
	strncpy(record->title,title,sizeof(title));
	strncpy(record->artist,artist,sizeof(artist));
	record->track_count = 0;

	add_record(record);
	return 0;
}

void add_record_ui()
{
	char title[MAX_LEN] = {0};
	char artist[MAX_LEN] = {0};
	char cd_entry[MAX_LEN] = {0};
	Record *record = NULL;
	record = (struct Record *)malloc(sizeof(struct Record));

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
		ret=add_record_wrap(title,artist,record);
		if(ret == 0){
			strncpy(current_cd, title, sizeof(title));
		}else{
			mvprintw(Q_LINE,start_col, "%s exsits.",title);
			clrtoeol(); /*delete chars behind 'exsits'*/
			refresh();
			sleep(1);
		}
	}
	
}

/* Ending of add record*/

	
/*Beginning of remove record*/
int remove_record(Record *record)
{
    if(record_list.record_cnt)
        record_list.record_cnt -= 1;

    list_del(&record->list);
    return 0;
}

int remove_record_wrap(char *title)
{
	Record *record = NULL;
	record = get_record_by_title(title);
	if(NULL == record)
		return 1;
	remove_record(record);
	free(record);
	return 0;
}	

void remove_record_ui()
{
	
	int ret = 0;
	char title[MAX_LEN] = {0};

	mvprintw(PROMPT_LINE, 0, "About to remove Record: %s", current_cd);

	if(!get_confirm())
		return;

	strncpy(title,current_cd,sizeof(title));

	ret = remove_record_wrap(title);
	if(ret == 1){ 
		mvprintw(ERROR_LINE, 0, "Failed");
		clrtoeol();
		refresh();
		sleep(1);
	}
		
	current_cd[0] = '\0';
	refresh();
}

/*Ending of remove record*/


/*Beginning of add track*/
int add_track(Record *record, Track *track)
{

	record->track_count += 1;
	list_add_tail(&track->list,&record->track);
	return 0;
}

int add_track_wrap(char *rtitle, char *title, char *style) {

	if(!rtitle){
		rtitle = (char *)malloc(MAX_LEN * sizeof(char));
		strncpy(rtitle,current_cd,sizeof(current_cd));
	}	
	
	Record *record = get_record_by_title(rtitle);
	if(NULL == record)
		return 1;

	Track *track = (struct Track *)malloc(sizeof(struct Track));
	*track = (struct Track){
	    .list = LIST_HEAD_INIT(track->list)
	};
	strncpy(track->title,title,sizeof(title));
	strncpy(track->style,style,sizeof(style));
	
	add_track(record,track);
	return 0;
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

	remove_all_tracks_of_one_record(NULL);

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
			int ret = add_track_wrap(NULL, title, "jazz");
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

int remove_track(Record *record,Track *track)
{
    if(record->track_count)
        record->track_count -= 1;
    list_del(&track->list);
    return 0; 
	
}

int remove_all_tracks_of_one_record(char *rtitle)
{
	if(!rtitle){
		rtitle = (char *)malloc(MAX_LEN * sizeof(char));
		strncpy(rtitle, current_cd, sizeof(current_cd));
	}

	Record *record = get_record_by_title(rtitle);
	if(record == NULL)
		return 1;

	struct list_head *pos,*n = NULL;
    list_for_each_safe(pos, n, &record->track){
        Track *track = container_of(pos, struct Track, list);
		remove_track(record,track);
		free(track);
	}

	return 0;
	
}

Record *get_record_by_title(char *title)
{
    struct list_head *pos,*n = NULL;
    struct Record *record = NULL;

    list_for_each_safe(pos, n, &record_list.list){
        record = container_of(pos, struct Record, list);
        if(strncmp(title,record->title,sizeof(record->title)) == 0){
			return record;
        }
    }

    return NULL;
}


void find_record_ui()
{
		
	Record *record = NULL;
	char title[MAX_LEN] = {0};

	mvprintw(Q_LINE, 0,"%s","Enter the title to search for:");
	getstr(title);

	refresh();

	record = get_record_by_title(title);

	if(NULL != record){

		strncpy(current_cd, title, sizeof(title));

	} else{

		mvprintw(ERROR_LINE, 0, "Sorry, no matching record found. ", title);
		get_return();
	}

	refresh();

}


int check_track_by_title(char *title)
{
    struct list_head *pos1, *n1 = NULL;
    list_for_each_safe(pos1, n1, &record_list.list){
        struct list_head *pos2, *n2 = NULL;
        struct Record *record = container_of(pos1, struct Record, list);
        list_for_each_safe(pos2, n2, &record->track){
            struct Track *track = container_of(pos2, struct Track, list);
            if(strncmp(title, track->title, sizeof(title)) == 0){
				return 0;
            }
        }
    }

	return 1;
	
}


int get_track_by_title_of_all(char *title)
{
    struct list_head *pos,*n = NULL;
    struct Record *record = NULL;

    /*initialize list by deleting the result from last traverse*/
    list_for_each_safe(pos, n, &record_result_list){
        record = container_of(pos, struct Record, list);
        list_del(&record->list);
        free(record);
        record = NULL;
    }

    pos,n = NULL;

    /*initialize list by deleting the result from last traverse*/
    list_for_each_safe(pos, n, &track_result_list){
        struct Track *track = container_of(pos, struct Track, list);
        list_del(&track->list);
        free(track);
        track = NULL;
    }

    struct list_head *pos1, *n1 = NULL;
    list_for_each_safe(pos1, n1, &record_list.list){
        struct list_head *pos2, *n2 = NULL;
        struct Record *record = container_of(pos1, struct Record, list);
        list_for_each_safe(pos2, n2, &record->track){
            struct Track *track = container_of(pos2, struct Track, list);
            if(strncmp(title, track->title, sizeof(title)) == 0){
                Record *r_cpy = (struct Record *)malloc(sizeof(struct Record));
		/*an element cannot belong to two list,so copy one*/
                memcpy(r_cpy,record,sizeof(struct Record));
                Track *t_cpy = (struct Track *)malloc(sizeof(struct Track));
                memcpy(t_cpy,track,sizeof(struct Track));
                list_add(&r_cpy->list, &record_result_list);
                list_add(&t_cpy->list, &track_result_list);
            }
        }
    }

	if(list_empty(&track_result_list)){
		return 1;
	}

	return 0;

}


Track *get_track_by_title_of_record(Record *record, char *ttitle)
{
	struct list_head *pos, *n = NULL;
	if(list_empty(&record->track))
		return NULL;

	list_for_each_safe(pos, n, &record->track){
		struct Track *track = container_of(pos, struct Track, list);
		if(strncmp(ttitle,track->title,sizeof(ttitle)) == 0)
			return track;
	}

	return NULL;
	

}

void list_track_by_title_of_record_ui(int start_row, int start_col, Record *record)
{
	struct list_head *pos, *n = NULL;
	int row_pos = 1;
	if(list_empty(&record->track)){
		mvprintw(start_row + row_pos, start_col, "%s has no tracks.", record->title);
		refresh();
		get_return();
		return;
	}
	
	mvprintw(start_row, start_col, "%s%s","Record Title: ",record->title);
	mvprintw(start_row + 1, start_col, "Track Counts: %d",record->track_count);

	mvprintw(start_row + 3 , start_col+1, "Track%12sStyle\n"," ");

	list_for_each_safe(pos, n, &record->track){
		struct Track *track = container_of(pos, struct Track, list);
		mvprintw(start_row + 3 + row_pos, start_col+1, "%-17s%s",track->title,track->style);
		row_pos++;
	}

	refresh();
	get_return();

}


void list_track_ui_ver1(int start_row, int start_col)
{
		
	Record *record = NULL;
	clear_all_screen();
	mvprintw(start_row - 2, start_col,"%s","Track list of current Record:");
	record = get_record_by_title(current_cd);
	if(NULL == record){
		mvprintw(start_row + 2, start_col, "%s not exsit, choose another record.", current_cd);
	}
	list_track_by_title_of_record_ui(start_row, start_col, record); 
}


void list_track_ui_ver2(int start_row, int start_col)
{

	int try = 3;
	Record *record = NULL;
	char title[MAX_LEN] = {0};


	while(try){
		clear_all_screen();
		mvprintw(start_row - 2, start_col,"%s","Track list of seraching Record:");
		mvprintw(start_row, start_col, "%s","Record Title:");

		getstr(title);
	
		record = get_record_by_title(title);
		if(NULL != record){
			break;
		}

		mvprintw(start_row + 2, start_col, "%s not exsit, choose another record.", title);

		if(try > 1)
			mvprintw(start_row + 3, start_col, "%d times left", try - 1);

		refresh();

		try--;

		sleep(2);

	}

	if (!try){
		return;
	}
	
	list_track_by_title_of_record_ui(start_row + 1, start_col, record); 
	
}

void list_track_ui()
{
	int start_row = 8, start_col = 10;
	if(current_cd[0]){
		list_track_ui_ver1(start_row, start_col);
	}else{
		list_track_ui_ver2(start_row, start_col);
	}
	
}



void display_all_records_ui(void)
{
	int start_row = 5, start_col = 10;
	int ret = 0;
	int row_pos = 1;
    struct list_head *pos,*n = NULL;

	clear();

	mvprintw(start_row - 2, start_col,"%s","Display all Records:");

	mvprintw(start_row, start_col, "Record%10sArtist%10sTrack Counts\n","","");
    list_for_each_safe(pos, n, &record_list.list){
    	struct Record *record = container_of(pos, struct Record, list);
		mvprintw(start_row + row_pos, start_col,"%-16s%-16s%d\n",record->title,record->artist,record->track_count);
		row_pos++;
    }

	refresh();
	get_return();
	
}

int display_track_of_record(Record *record)
{
		
    struct list_head *pos,*n = NULL;
	
	printf("Track%10sStyle%10sRecord\n","","");

	if(list_empty(&record->track))
		return 1;

    list_for_each_safe(pos, n, &record->track){
    	struct Track *track = container_of(pos, struct Track, list);
		printf("%-15s%-15s%s\n",track->title,track->style,record->title);
    }

	return 0;
}



int get_track_data(char *rtitle, char *ttitle, char *tstyle)
{
	printf("Record Title: ");
	scanf("%s",rtitle);

	printf("Track Title: ");
	scanf("%s",ttitle);

	printf("Track Style: ");
	scanf("%s",tstyle);
	return 0;
	
}

void draw_menu_color(char *choice[], int selected, int start_row, int start_col)
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

void draw_menu(char *choice[], int selected, int start_row, int start_col)
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


int get_confirm()
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

void clear_all_screen()
{
	clear();
	mvprintw(2, 20, "CD Database Application");
	if(current_cd[0]){
		mvprintw(ERROR_LINE, 0, "Current Record: %s\n",current_cd);
	}
	refresh();
}




/*curse color mode*/
void start_color_mode()
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

int getchoice(char *greet, char *choice[])
{
	
	int key = 0;
	int selected = 1;
	int option_cnt = 0;
	int start_row = 5, start_col = 10;
	char **options;


	options = choice;

	while(*options){
		options++;
		option_cnt++;
	}
	
	clear_all_screen();

	noecho();
	cbreak();

	mvprintw(start_row - 2,start_col,"%s",greet);
	
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

void get_return()
{
	int ch;
	mvprintw(ERROR_LINE + 1, 0, "Press return ");
	refresh();
	while((ch = getchar()) != '\n' && ch != EOF);
}
