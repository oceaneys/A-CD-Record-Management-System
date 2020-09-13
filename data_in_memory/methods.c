#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "data.h"
#include "list.h"

 RecordList extern record_list={0,LIST_HEAD_INIT(record_list.list)};

/*two global list created for `find_*_by_title`*/
LIST_HEAD(record_result_list); 
LIST_HEAD(track_result_list);


int init_record_list()
{
    Record record1 = { .title="title1",
        .artist="heavy1",
        .track_count=0,
        .track=LIST_HEAD_INIT(record1.track),
        .list=LIST_HEAD_INIT(record1.list)
    };
    Record *record2 = (struct Record *)malloc(sizeof(struct Record));
    *record2 = (struct Record){ .title="title2",
        .artist="heavy2",
        .track_count=2,
        .track=LIST_HEAD_INIT(record2->track),
        .list=LIST_HEAD_INIT(record2->list)
    };
    add_record(&record1);
    add_record(record2);

    Track track1 = {
        .title = "track1",
        .style = "style1",
        .list = LIST_HEAD_INIT(track1.list)
    };

    Track track2 = {
        .title = "track2",
        .style = "style2",
        .list = LIST_HEAD_INIT(track2.list)
    };

    Track track3 = {
        .title = "track2",
        .style = "style3",
        .list = LIST_HEAD_INIT(track3.list)
    };
    add_track(&record1,&track1);
    add_track(&record1,&track2);
    add_track(&record1,&track3);
}

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

    /*initialize list by deleting the result from last traverse*/
    list_for_each_safe(pos, n, &record_result_list){
        record = container_of(pos, struct Record, list);
        list_del(&record->list);
        free(record);
        record = NULL;
    }

    pos,n = NULL;
    list_for_each_safe(pos, n, &record_list.list){
        record = container_of(pos, struct Record, list);
        if(strncmp(title,record->title,sizeof(title)) == 0){
            Record *tmp = (struct Record *)malloc(sizeof(struct Record));
            memcpy(tmp,record,sizeof(record));
            list_add(&tmp->list,&record_result_list);
        }
    }

	if(list_empty(&record_result_list)){
		return 1;
	}
    return 0;
}

int find_track_by_title(char *title)
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

int display_all_records(void)
{
    struct list_head *pos,*n = NULL;
	
	printf("Record%10sArtist%10sTrack Counts\n","","");

	if(list_empty(&record_list.list))
		return 1;

    list_for_each_safe(pos, n, &record_list.list){
    	struct Record *record = container_of(pos, struct Record, list);
		printf("%-16s%-16s%d",record->title,record->artist,record->track_count);
    }

	return 0;
	
}