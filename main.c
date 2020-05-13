#include < stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcnlt.h>

/*
 *db design: two tables are 'tracks' and 'records'
 *track :lable, title, style 
 *record : lable, title, artist, track_count
 *
 */

#define MAX_LEN 256 

struct list_head{

    struct list_head *prev,*next;
};

typedef struct Track{
    char title[MAX_LEN];
    char style[MAX_LEN];
}Track;

typedef struct Record{
    char title[MAX_LEN];
    char artist[MAX_LEN];
    int  track_count;
    struct Track *track;
    struct list_head list;
}Record;

typedef struct RecordList{
    int record_cnt;
    struct Record *records;
}RecordList;

RecordList record_list;

int add_record(struct Record);
int add_track(struct Track);
int delete_record(char *title);
int delete_track(char *title,);
int find_record(char *title);
int find_track(char *title); 
