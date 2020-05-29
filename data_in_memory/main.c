#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "list.h"

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};
LIST_HEAD(record_result_list);
LIST_HEAD(track_result_list);

int add_record(struct Record *);
int add_track(struct Record *, struct Track *);
int remove_record(struct Record *record);
int remove_track(struct Record *, struct Track *);
int find_record_by_title(char *title);
int find_track_by_title(char *title); 

int add_record(Record *record)
{
   record_list.record_cnt += 1; 
   list_add_tail(&record->list,&record_list.list);
   return 0;
} 

int remove_record(Record *record)
{
    if(record_list.record_cnt)
        record_list.record_cnt -= 1;

    list_del(&record->list);
    return 0;
}


int add_track(Record *record, Track *track)
{
   record->track_count += 1;
   list_add_tail(&track->list,&record->track);
   return 0;
}

int remove_track(Record *record,Track *track)
{
    if(record->track_count)
        record->track_count -= 1;
    list_del(&track->list);
    return 0;
}

int find_record_by_title(char *title)
{
    struct list_head *pos,*n = NULL;
    struct Record *record = NULL;
    list_for_each_safe(pos, n, &record_list.list){
        record = container_of(pos, struct Record, list);
        if(strncmp(title,record->title,sizeof(title)) == 0)
            list_add(&record->list,&record_result_list);
    }
    return 0;
}

int main(int argc, char **argv)
{

}

