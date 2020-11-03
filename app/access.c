#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <ndbm.h>
#include <mysql.h>

#include "include/access.h"

#define RECORD_DB_FILE_BASE  "record_db"
#define RECORD_DB_FILE_DIR  "record_db.dir"
#define RECORD_DB_FILE_PAG  "record_db.pag"

#define TRACK_DB_FILE_BASE  "track_db"
#define TRACK_DB_FILE_DIR  "track_db.dir"
#define TRACK_DB_FILE_PAG  "track_db.pag"

static DBM *record_db_ptr = NULL;
static DBM *track_db_ptr = NULL;

static MYSQL mysqlconn;
#define RECORD_SQL_TABLE "blpcd.cd"
#define TRACK_SQL_TABLE "blpcd.track"
#define ARTIST_SQL_TABLE "blpcd.track"


static int remove_item_db(MYSQL *, const char *);
static int add_item_db(MYSQL *, const char *);

static int get_record_id(const char *);
static int count_tracks_record(char *);
static Bool track_exsits(char *rtitle,int track_no);
static int remove_track_wrap(char *rtitle, int track_no);


/*Begining of db manipulation*/
int db_initialize()
{
	mysql_init(&mysqlconn);
	if(!mysql_real_connect(&mysqlconn, 
						  "localhost", 
						  "rick",
						  "rickbuddy",
						  "blpcd",
						  0,
						  NULL,
						  0)){
	if(mysql_errno(&mysqlconn))
		fprintf(stderr,"Connection error %d: %s\n",
						mysql_errno(&mysqlconn),mysql_error(&mysqlconn));
	return 1;
	}
	return 0;
}

void db_close()
{
	if(&mysqlconn)
		mysql_close(&mysqlconn);
}

static int remove_item_db(MYSQL *db, const char *sql)
{	
	int ret;
	ret = mysql_query(db, sql);
	if(ret != 0)
		return 1;
    return 0; 
}

static int add_item_db(MYSQL *db, const char *sql)
{
	int ret;		
	ret = mysql_query(db, sql);
	if(ret != 0)
		return 1;
	return 0;
}

/*End of db manipulation*/




/*Beginning of record manipulation*/

Bool record_exsits(char *title)
{
	if(get_record_id(title) == -1)
		return False;
	return True;
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

static int get_artist_id(const char *artist)
{
	int ret;
	MYSQL_RES *mysql_res;
	MYSQL_ROW mysql_row;
	char sql[MAX_LEN];
	int artist_id = -1;

	sprintf(sql, "SELECT id FROM artist WHERE name = '%s'",artist);
	ret = mysql_query(&mysqlconn,sql);
	if(ret){
		fprintf(stderr,"SELECT error: %s",mysql_error(&mysqlconn));
	}else{
		mysql_res = mysql_store_result(&mysqlconn);
		if(mysql_res){
			if(mysql_num_rows(mysql_res) > 0){
				if(mysql_row = mysql_fetch_row(mysql_res)){
					sscanf(mysql_row[0], "%d", &artist_id);
				}
				
			}
			mysql_free_result(mysql_res);
		}
	}

	if(artist_id != -1)return artist_id;

	/*The 'artist' doesn't exsit, so we insert it into artist table*/
	memset(sql,'\0',sizeof(sql));
	sprintf(sql, "INSERT INTO artist(name) VALUES('%s')",artist);
	ret = mysql_query(&mysqlconn,sql);
	if(ret){
		fprintf(stderr,"INSERT error %d: %s",mysql_errno(&mysqlconn),mysql_error(&mysqlconn));
		return 0;
	}
	
	memset(sql,'\0',sizeof(sql));
	ret = mysql_query(&mysqlconn, "SELECT LAST_INSERT_ID()");
	if(ret){
		fprintf(stderr,"SELECT error: %s",mysql_error(&mysqlconn));
	}else{
		mysql_res = mysql_store_result(&mysqlconn);
		if(mysql_res){
			if(mysql_num_rows(mysql_res) > 0){
				if(mysql_row = mysql_fetch_row(mysql_res)){
					sscanf(mysql_row[0],"%d",&artist_id);
				}
			}
		}
		mysql_free_result(mysql_res);
	}

	return artist_id;
}

int add_record_wrap(char *title, char *artist)
{

	int ret;
	char insql[MAX_LEN];
	int artist_id;

	memset(insql,'\0',sizeof(insql));

	artist_id = get_artist_id(artist);
	sprintf(insql,"INSERT INTO cd(title, artist_id) VALUES('%s', %d)",title, artist_id);
	
	ret = add_item_db(&mysqlconn,insql);
	return ret;
}

int remove_record_wrap(char *title)
{
	int ret;
	char delsql[MAX_LEN];

	memset(delsql, '\0', sizeof(delsql));

	sprintf(delsql, "DELETE FROM cd WHERE title = '%s'",title);
	ret = remove_item_db(&mysqlconn,delsql);
	return ret;
}	

/*End of record manipulation*/


/*Beginning of track maniplulation*/

static Bool track_exsits(char *rtitle,int track_no)
{
	/*TO do*/
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

static int get_record_id(const char *title)
{
	int ret;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char insql[MAX_LEN];
	int cd_id = -1;
	
	memset(insql,'\0',sizeof(insql));
	sprintf(insql,"SELECT id FROM cd WHERE title = '%s'",title);

	ret = mysql_query(&mysqlconn,insql);
	if(ret){
		fprintf(stderr,"SELECT error %d: %s", mysql_errno(&mysqlconn),mysql_error(&mysqlconn));
		return -1;
	}else{
		res = mysql_store_result(&mysqlconn);
		if(res){
			if(mysql_num_rows(res) > 0){
				if(row = mysql_fetch_row(res)){
					sscanf(row[0],"%d",&cd_id);
				}	
			}
		}
		mysql_free_result(res);
	}
	return cd_id;
}

int add_track_wrap(char *title, char *style, int track_no) {

	int ret;
	int cd_id;
	char insql[MAX_LEN]; 

	cd_id = get_record_id(current_cd);
	if(-1 == cd_id)
		return 1;

	memset(insql,'\0',sizeof(insql));
	sprintf(insql, "INSERT INTO track(cd_id, track_id, title) VALUES(%d,%d,'%s')",cd_id,track_no,title);
	ret = add_item_db(&mysqlconn,insql);
	return ret;
}



/*We take all tracks as an whole stuff, and use `remove_all_tracks_of_one_record` to remove all of them.
 *So,we are not going to use this function for a long time.
 * */
static int remove_track_wrap(char *rtitle, int track_no) 
{
	/*To to*/
	return 0;
}

/*for testing, waiting for 'remove_item_db' get ready*/
int remove_all_tracks_of_one_record(char *rtitle)
{
	int ret;
	char delsql[MAX_LEN];
	int cd_id;
	cd_id = get_record_id(current_cd);
	if(-1 == cd_id)
		return 1;

	memset(delsql,'\0',sizeof(delsql));
	sprintf(delsql,"DELETE FROM track WHERE cd_id = %d",cd_id);
	ret = remove_item_db(&mysqlconn,delsql);
	return ret;
}

/*End of track maniplulation*/

void fulfill_current_cd(char *string)
{
	strncpy(current_cd, string, strlen(string));
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


