#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <ndbm.h>

#include "include/access.h"
#include "include/list.h"

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};
LIST_HEAD(record_result_list);
LIST_HEAD(track_result_list);

typedef enum Bool 
{
	False = 0,
	True = 1

}Bool;

#define RECORD_DB_FILE_BASE  "record_db"
#define RECORD_DB_FILE_DIR  "record_db.dir"
#define RECORD_DB_FILE_PAG  "record_db.pag"

#define TRACK_DB_FILE_BASE  "track_db"
#define TRACK_DB_FILE_DIR  "track_db.dir"
#define TRACK_DB_FILE_PAG  "track_db.pag"

static DBM *record_db_ptr = NULL;
static DBM *track_db_ptr = NULL;

static bool record_exsits(char *);

int db_initialize(int new_database)
{
	int open_mode = O_RDWR | O_CREAT;

	if(record_db_ptr) dbm_close(record_db_ptr);
	if(track_db_ptr) dbm_close(track_db_ptr);

	if(new_database){
		unlink(RECORD_DB_FILE_DIR);
		unlink(RECORD_DB_FILE_PAG);

		unlink(TRACK_DB_FILE_DIR);
		unlink(TRACK_DB_FILE_PAG);
	}
	
	record_db_ptr = dbm_open(RECORD_DB_FILE_BASE, open_mode, 0666);
	track_db_ptr = dbm_open(TRACK_DB_FILE_BASE, open_mode, 0666);
	if(!record_db_ptr || !track_db_ptr){
		record_db_ptr = track_db_ptr = NULL;
		return 1;
	}

	return 0;
		
}

void db_close()
{
	if(record_db_ptr) dbm_close(record_db_ptr);
	if(track_db_ptr) dbm_close(track_db_ptr);

	record_db_ptr = track_db_ptr = NULL;
}


static bool record_exsits(char *title)
{
	datum ret,key;
	key.dptr = (void *)title;
	key.dsize = sizeof(title);
	ret = dbm_fetch(record_db_ptr, key);
	if(ret.dptr) return True;
	return False;

}

/*Beginning of add record*/

static int add_record_db(Record *record)
{
	char key_to_add[MAX_LEN - 1];
	datum local_key_datum;
	datum local_data_datum;
	int ret;

	memset(key_to_add, '\0', sizeof(key_to_add));
	strcpy(key_to_add, record->title);

	local_key_datum.dptr = (void *)key_to_add;
	local_key_datum.dsize = sizeof(key_to_add);
	local_data_datum.dptr = (void *)record;
	local_data_datum.dsize = sizeof(struct Record);

	ret = dbm_store(record_db_ptr, local_key_datum, local_data_datum, DBM_REPLACE);
	if(ret == 0)
		return ret;

	return 1;
} 


int add_record_wrap(char *title, char *artist)
{

	int ret;

	if(record_exsits(title)){
		return 1;
	}

	Record *record = (Record *)malloc(sizeof(struct Record));
	strncpy(record->title,title,sizeof(title));
	strncpy(record->artist,artist,sizeof(artist));
	record->track_count = 0;

	ret = add_record_db(record);
	return ret;
}


/* Ending of add record*/

/*Beginning of remove record*/
static int remove_record(Record *record)
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


/*Ending of remove record*/


/*Beginning of add track*/
static int add_track(Record *record, Track *track)
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


static int remove_track(Record *record,Track *track)
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


static int check_track_by_title(char *title)
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


static int get_track_by_title_of_all(char *title)
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


static Track *get_track_by_title_of_record(Record *record, char *ttitle)
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

int list_track_by_title_of_record(int start_row, int start_col, char *rtitle)
{
	struct list_head *pos, *n = NULL;
	int row_pos = 1;

	Record *record = get_record_by_title(rtitle);

	if(NULL == record){
		mvprintw(start_row + 2, start_col, "%s not exsit, choose another record.", current_cd);
		return 1;
	}

	if(list_empty(&record->track)){
		mvprintw(start_row + row_pos, start_col, "%s has no tracks.", record->title);
		refresh();
		return 1;
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
	return 0;

}

void display_all_records(int start_row, int start_col)
{
	int row_pos = 1;
    struct list_head *pos,*n = NULL;

	mvprintw(start_row - 2, start_col,"%s","Display all Records:");

	mvprintw(start_row, start_col, "Record%10sArtist%10sTrack Counts\n","","");
    list_for_each_safe(pos, n, &record_list.list){
    	struct Record *record = container_of(pos, struct Record, list);
		mvprintw(start_row + row_pos, start_col,"%-16s%-16s%d\n",record->title,record->artist,record->track_count);
		row_pos++;
    }

	refresh();
}

static int display_track_of_record(Record *record)
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


void fulfill_current_cd(char *string)
{
	strncpy(current_cd, string, sizeof(string));
}

void zero_current_cd()
{
	current_cd[0] = '\0';
}

char *get_current_cd()
{
	return current_cd;
}

char get_current_cd0()
{
	return current_cd[0];
}

