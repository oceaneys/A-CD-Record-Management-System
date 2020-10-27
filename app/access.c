#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <ndbm.h>

#include "include/access.h"

#define RECORD_DB_FILE_BASE  "record_db"
#define RECORD_DB_FILE_DIR  "record_db.dir"
#define RECORD_DB_FILE_PAG  "record_db.pag"

#define TRACK_DB_FILE_BASE  "track_db"
#define TRACK_DB_FILE_DIR  "track_db.dir"
#define TRACK_DB_FILE_PAG  "track_db.pag"

static DBM *record_db_ptr = NULL;
static DBM *track_db_ptr = NULL;

static int remove_item_db(DBM *db, datum key);
static int add_item_db(DBM *db, datum key, datum data);

static Record *get_record_by_title(char *);
static int count_tracks_record(char *);
static Bool track_exsits(char *rtitle,int track_no);
static int remove_track_wrap(char *rtitle, int track_no);


/*Begining of db manipulation*/
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



static int remove_item_db(DBM *db, datum key)
{
	int ret;
	ret = dbm_delete(db,key);
	if(ret != 0) 
		return 1;
    return 0; 
}

static int add_item_db(DBM *db, datum key, datum data)
{
	int ret;		
	ret = dbm_store(db, key, data, DBM_REPLACE);
	if(ret != 0)
		return 1;
	return 0;
}

/*End of db manipulation*/




/*Beginning of record manipulation*/

Bool record_exsits(char *title)
{
	datum ret,key;
	key.dptr = (void *)title;
	key.dsize = strlen(title);
	ret = dbm_fetch(record_db_ptr, key);
	if(ret.dptr) return True;
	return False;

}

static Record *get_record_by_title(char *title)
{
	if(!record_exsits(title))
		return NULL;

	datum data,key;
	key.dptr = (void *)title;
	key.dsize = strlen(title);

	data = dbm_fetch(record_db_ptr,key);
	if(!data.dptr)
		return NULL;

	return((struct Record *)data.dptr);
}

void display_all_records(int start_row, int start_col)
{
	datum key;
	datum data;
	int row_pos = 1;

	mvprintw(start_row - 2, start_col,"%s","Display all Records:");
	mvprintw(start_row, start_col, "Record%10sArtist%10sTrack Counts\n","","");

	Record *record = NULL;

	for(key = dbm_firstkey(record_db_ptr); key.dptr; key = dbm_nextkey(record_db_ptr)){
		data = dbm_fetch(record_db_ptr,key);	
		if(!data.dptr)
			continue;
		record = (Record *)data.dptr;
		mvprintw(start_row + row_pos, start_col,"%-16s%-16s%d\n",record->title,record->artist,count_tracks_record(record->title));
		row_pos++;
    }

	refresh();
}

int add_record_wrap(char *title, char *artist)
{

	int ret;
	char key_to_use[MAX_LEN];
	datum key,data;

	if(record_exsits(title)){
		return 1;
	}

	Record *record = (Record *)malloc(sizeof(struct Record));
	strncpy(record->title,title,sizeof(title));
	strncpy(record->artist,artist,sizeof(artist));
	record->track_count = 0;

	memset(key_to_use,'\0',sizeof(key_to_use));
	strcpy(key_to_use,record->title);	
	
	key.dptr = (void *)key_to_use;
	key.dsize = strlen(record->title);

	data.dptr = (void *)record;
	data.dsize = sizeof(struct Record);
	
	ret = add_item_db(record_db_ptr,key,data);
	return ret;
}

int remove_record_wrap(char *title)
{
	int ret;
	datum key;
	char key_to_use[MAX_LEN];

	if(!record_exsits(title))
		return 1;
	
	memset(key_to_use,'\0',sizeof(key_to_use));
	strcpy(key_to_use,title);	
	
	key.dptr = (void *)key_to_use;
	key.dsize = strlen(title);

	ret = remove_item_db(record_db_ptr,key);
	return ret;
}	

/*End of record manipulation*/


/*Beginning of track maniplulation*/

static Bool track_exsits(char *rtitle,int track_no)
{
	char key_to_use[MAX_LEN];
	datum ret,key;

	memset(key_to_use,'\0',sizeof(key_to_use));
	sprintf(key_to_use,"%s_%d",rtitle,track_no);

	key.dptr = (void *)key_to_use;
	key.dsize = strlen(key_to_use);

	ret = dbm_fetch(track_db_ptr,key);
	if(!ret.dptr)
		return False;
    return True; 

}

int list_track_by_title_of_record(int start_row, int start_col, char *rtitle)
{
	int row_pos = 1;


	if(!record_exsits(rtitle)){
		mvprintw(start_row + 2, start_col, "%s not exsit, choose another record.", rtitle);
		return 1;
	}
	
	mvprintw(start_row, start_col, "%s%s","Record Title: ",rtitle);

	mvprintw(start_row + 2 , start_col+1, "Track%12sStyle\n"," ");
	
	int ret;
	datum key,data;
	char key_to_use[MAX_LEN];
	Track *track = NULL;
	for(key = dbm_firstkey(track_db_ptr);key.dptr;key=dbm_nextkey(track_db_ptr)){
	
		if((ret = strncmp(rtitle, (char *)key.dptr, strlen(rtitle))) != 0)
			continue;

		data = dbm_fetch(track_db_ptr,key);
		if(!data.dptr) 
			return 0; 
		track = (Track *)data.dptr;
		mvprintw(start_row + 3 + row_pos, start_col+1, "%-17s%s",track->title,track->style);
		row_pos++;
	}
	refresh();
	return 0;

}

static int count_tracks_record(char *rtitle)
{
	int ret;
	datum key;
	int track_cnt = 0;
	for(key = dbm_firstkey(track_db_ptr);key.dptr;key=dbm_nextkey(track_db_ptr)){
		if((ret = strncmp(rtitle, (char *)key.dptr, strlen(rtitle))) != 0)
			continue;

		track_cnt++;
	}

	return track_cnt;

}
int add_track_wrap(char *title, char *style, int track_no) {

	int ret;
	char key_to_use[MAX_LEN];
	datum key,data;

	Record *record = get_record_by_title(current_cd);
	if(NULL == record)
		return 1;

	Track *track = (struct Track *)malloc(sizeof(struct Track));
	strncpy(track->rtitle, current_cd, sizeof(current_cd));
	strncpy(track->title, title, sizeof(title));
	strncpy(track->style, style, sizeof(style));
	track->track_no = track_no;
	
	memset(key_to_use, '\0', sizeof(key_to_use));
	sprintf(key_to_use, "%s_%d", track->rtitle,track->track_no);

	key.dptr = (void *)key_to_use;
	key.dsize = strlen(key_to_use);

	data.dptr = (void *)track;
	data.dsize = sizeof(struct Track);

	ret = add_item_db(track_db_ptr,key,data);
	return ret;
}



/*We take all tracks as an whole stuff, and use `remove_all_tracks_of_one_record` to remove all of them.
 *So,we are not going to use this function for a long time.
 * */
static int remove_track_wrap(char *rtitle, int track_no) {

	int ret;
	char key_to_use[MAX_LEN];
	datum key;

	memset(key_to_use,'\0',sizeof(key_to_use));
	sprintf(key_to_use,"%s_%d",rtitle,track_no);

	if(!track_exsits(rtitle,track_no))
		return 0;

	key.dptr = (void *)key_to_use;
	key.dsize = strlen(key_to_use);

	ret = remove_item_db(track_db_ptr,key);

	return ret;
	
}

int remove_all_tracks_of_one_record(char *rtitle)
{

	if(!record_exsits(rtitle))
		return 1;
	
	datum key;
	for(key = dbm_firstkey(track_db_ptr); key.dptr; key = dbm_nextkey(track_db_ptr)){
		if(strncmp(rtitle,(char *)key.dptr,strlen(rtitle)) != 0)
			continue;
		remove_item_db(track_db_ptr,key);
		key = dbm_firstkey(track_db_ptr);
	}
	return 0;
	
	
}

/*End of track maniplulation*/

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


