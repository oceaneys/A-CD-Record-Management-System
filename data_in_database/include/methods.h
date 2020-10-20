#ifndef INCLUDE_METHODS_H
#define INCLUDE_METHODS_H 

#define MAX_LEN 256

char current_cd[MAX_LEN];

/*simple doubly linked list implementation.*/
struct list_head{
    struct list_head *prev,*next;
};

typedef struct Track{
    struct list_head list;
    char title[MAX_LEN];
    char style[MAX_LEN];
}Track;

typedef struct Record{
    struct list_head list;
    char title[MAX_LEN];
    char artist[MAX_LEN];
    int  track_count;
    struct list_head track;
}Record;

typedef struct RecordList{
    int record_cnt;
    struct list_head list;
}RecordList;

int add_record_wrap(char *, char *);
int remove_record_wrap(char *);
Record *get_record_by_title(char *title);
void display_all_records(int, int);

int list_track_by_title_of_record(int, int, char *);
int add_track_wrap(char *rtitle, char *title, char *style);

void zero_current_cd(); /* current_cd[0] = '\0' */
void fulfill_current_cd(char *);
char *get_current_cd();
char get_current_cd0();

#endif
