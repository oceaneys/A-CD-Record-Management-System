#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <curses.h>

#include "data.h"
#include "list.h"

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};

/*two global list created for `find_*_by_title`*/
LIST_HEAD(record_result_list); 
LIST_HEAD(track_result_list);

int add_record(char *title, char *artist, Record *record)
{
	*record = (struct Record){ 
		.title=title,
	    .artist=artist,
	    .track_count=0,
	    .track=LIST_HEAD_INIT(record->track),
	    .list=LIST_HEAD_INIT(record->list)
	};

	record_list.record_cnt += 1; 
	list_add_tail(&record->list,&record_list.list);
	return 0;
} 

/*
int add_record(Record *record)
{
   record_list.record_cnt += 1; 
   list_add_tail(&record->list,&record_list.list);
   return 0;
}
*/

int add_record_wrap(char *title, char *artist, Record *record)
{
	if(get_record_by_title(title)){
		return 1;
	}
	add_record(title,artist,record);
	return 0;
}

int remove_record(Record *record)
{
    if(record_list.record_cnt)
        record_list.record_cnt -= 1;

    list_del(&record->list);
    return 0;
}


int add_track(Record *record, char *title, char *style, Track *track)
{
	*track = (struct Track){
	    .title = title,
	    .style = style,
	    .list = LIST_HEAD_INIT(track->list)
	};
	record->track_count += 1;
	list_add_tail(&track->list,&record->track);
	return 0;
}

int add_track_wrap(char *rtitle, char *title, char *style, Track *track)
{
	
	Record *record = get_record_by_title(rtitle);
	if(NULL == record)
		return 1;

	if(get_track_by_title_of_record(record,title))
		return 0;
		
	add_track(record,title,style,track);
	return 0;
}

/*
int add_track(Record *record, Track *track)
{
   record->track_count += 1;
   list_add_tail(&track->list,&record->track);
   return 0;
}
*/

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

int display_all_records(void)
{
    struct list_head *pos,*n = NULL;
	
	printf("Record%10sArtist%10sTrack Counts\n","","");

	if(list_empty(&record_list.list))
		return 1;

    list_for_each_safe(pos, n, &record_list.list){
    	struct Record *record = container_of(pos, struct Record, list);
		printf("%-16s%-16s%d\n",record->title,record->artist,record->track_count);
    }

	return 0;
	
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


int get_record_data(char *rtitle, char *rartist)
{
	printf("Record Title: ");
	scanf("%s",rtitle);
	printf("Record Artist: ");
	scanf("%s",rartist);
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


void draw_menu(char *choice[], int selected, int start_row, int start_col)
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
	int selected = 0;
	int option_cnt = 0;
	int start_row = 5, start_col = 10;
	char **options;


	options = choice;

	while(*options){
		options++;
		option_cnt++;
	}

	selected = option_cnt;
	
	noecho();
	cbreak();

	mvprintw(start_row - 2,start_col,"%s",greet);
	
	keypad(stdscr, TRUE);

	while(key != ERR && key != KEY_ENTER ){

		if( key == KEY_DOWN && selected < option_cnt){

			selected++;
		}

		if( key == KEY_UP && selected > 1){
		
			selected--;
		}
		
		draw_menu(choice, selected, start_row, start_col);
		key = getch();
	}

	echo();
	nocbreak();

	return choice[selected-1][0]; 

}

