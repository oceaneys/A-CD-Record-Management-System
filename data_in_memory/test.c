#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "list.h"

RecordList record_list={0,LIST_HEAD_INIT(record_list.list)};

int add_record(struct Record *);
int add_track(struct Track *);
int delete_record(char *title);
int delete_track(char *title);
int find_record(char *title);
int find_track(char *title); 

int add_record(Record *record)
{
   record_list.record_cnt += 1; 
   list_add_tail(&record->list,&record_list.list);
   return 0;
} 

int main(int argc, char **argv)
{
    Record record1 = { .title="title1",
        .artist="heavy1",
        .track_count=1,
        .track=NULL,
        .list=LIST_HEAD_INIT(record1.list)
    };
    Record *record2 = (struct Record *)malloc(sizeof(struct Record));
    *record2 = (struct Record){ .title="title2",
        .artist="heavy2",
        .track_count=2,
        .track=NULL,
        .list=LIST_HEAD_INIT(record2->list)
    };
    Record *record3 = (struct Record *)malloc(sizeof(struct Record));
    *record3 = (struct Record){ .title="title3",
        .artist="heavy3",
        .track_count=3,
        .track=NULL,
        .list=LIST_HEAD_INIT(record3->list)
    };

    add_record(&record1);
    add_record(record2);
    add_record(record3);

    struct list_head *pos,*n = NULL;

    list_for_each_safe(pos,n,&record_list.list){
        Record *rtmp = container_of(pos,struct Record,list);
        printf("record.artist=%s\n",rtmp->artist);
    }

    list_del(&record2->list);

    list_for_each_safe(pos,n,&record_list.list){
        Record *rtmp = container_of(pos,struct Record,list);
        printf("record.artist=%s\n",rtmp->artist);
    }

    return 0;
}
