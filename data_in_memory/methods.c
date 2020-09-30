#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <curses.h>

#include "include/data.h"
#include "include/list.h"

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};

/*two global list created for `find_*_by_title`*/
LIST_HEAD(record_result_list); 
LIST_HEAD(track_result_list);

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
	Record *record = NULL;
	record = (struct Record *)malloc(sizeof(struct Record));

	int start_row = 5, start_col = 10;
	int ret = 0;

	clear();

	mvprintw(start_row - 2, start_col,"%s","Record Info Input:");

	mvprintw(start_row, start_col, "%s","Record Title:");
	getstr(title);

	mvprintw(start_row + 1, start_col, "%s","Record Artist:");
	getstr(artist);


	if((ret=add_record_wrap(title,artist,record)) == 0){
		strncpy(current_cd, title, sizeof(title));
		mvprintw(start_row + 3,start_col, "%s added.",title);
	}
	else
		mvprintw(start_row + 3,start_col, "%s exsits.",title);

	refresh();
	sleep(1);

	
}
	
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
	return 0;
}	

void remove_record_ui()
{
	
	int start_row = 5, start_col = 10;
	int ret = 0;
	char title[MAX_LEN] = {0};

	clear();

	mvprintw(start_row - 2, start_col,"%s","Remove a Record: ");

	mvprintw(start_row, start_col, "%s","Record Title:");
	getstr(title);

	if((ret=remove_record_wrap(title)) == 0)
		mvprintw(start_row + 3,start_col, "remove record, done.");
	else
		mvprintw(start_row + 3,start_col, "%s not exsits.\n",title);

	refresh();

	sleep(1);
}


int add_track(Record *record, Track *track)
{

	record->track_count += 1;
	list_add_tail(&track->list,&record->track);
	return 0;
}

int add_track_wrap(char *rtitle, char *title, char *style, Track *track)
{
	
	Record *record = get_record_by_title(rtitle);
	if(NULL == record)
		return 1;

	*track = (struct Track){
	    .list = LIST_HEAD_INIT(track->list)
	};
	strncpy(track->title,title,sizeof(title));
	strncpy(track->style,style,sizeof(style));

	if(get_track_by_title_of_record(record,title))
		return 0;
		
	add_track(record,track);
	return 0;
}

int remove_track(Record *record,Track *track)
{
    if(record->track_count)
        record->track_count -= 1;
    list_del(&track->list);
    return 0; 
	
}

Record *get_record_by_title(char *title)
{
    struct list_head *pos,*n = NULL;
    struct Record *record = NULL;

    list_for_each_safe(pos, n, &record_list.list){
        record = container_of(pos, struct Record, list);
        if(strncmp(title,record->title,sizeof(title)) == 0){
			return record;
        }
    }

    return NULL;
}


void find_record_ui()
{
		
	int start_row = 5, start_col = 10;
	Record *record = NULL;
	char title[MAX_LEN] = {0};

	clear();

	mvprintw(start_row - 2, start_col,"%s","Find a Record:");

	mvprintw(start_row, start_col, "%s","Record Title:");
	getstr(title);

	record = get_record_by_title(title);
	if(NULL != record)
		mvprintw(start_row + 2, start_col, "Record %s, Found.",title);	
	else
		mvprintw(start_row + 2, start_col, "Record %s Not exsit.", title);

	refresh();
	sleep(1);
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
		sleep(2);
		return;
	}
	
	list_for_each_safe(pos, n, &record->track){
		struct Track *track = container_of(pos, struct Track, list);
		mvprintw(start_row + row_pos, start_col, "Track-%d: %s", row_pos,track->title);
		row_pos++;
	}

	refresh();
	sleep(2);

}


void list_track_ui()
{
	int start_row = 5, start_col = 10;
	int try = 3;

	Record *record = NULL;
	char title[MAX_LEN] = {0};


	while(try){


		clear();
		mvprintw(start_row - 2, start_col,"%s","List Tracks of one Record:");

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
	
	list_track_by_title_of_record_ui(start_row + 2, start_col, record); 
	
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
	sleep(2);
	
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

	refresh();
		
}

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
	
	clear();

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

	echo();
	nocbreak();

	return choice[selected-1][0]; 

}

