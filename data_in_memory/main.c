#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include "list.h"

RecordList record_list={0,NULL};

int add_record(struct Record *);
int add_track(struct Track *);
int delete_record(char *title);
int delete_track(char *title);
int find_record(char *title);
int find_track(char *title); 

int add_record(Record *record)
{
   record_list.record_cnt += 1; 
   if(record_list.records == NULL){
        record_list.records = record;
        return 0;
   }

   list_add_tail(&record->list,&record_list.records->list);
   return 0;
} 

int main(int argc, char **argv)
{

}

