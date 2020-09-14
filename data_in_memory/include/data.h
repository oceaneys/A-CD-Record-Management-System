#define MAX_LEN 256 

/*simple doubly linked list implementation.*/

struct list_head{
    struct list_head *prev,*next;
};

typedef struct Track{
    struct list_head list;
    //char title[MAX_LEN];
    char *title;
    //char style[MAX_LEN];
    char *style;
}Track;

typedef struct Record{
    struct list_head list;
    //char title[MAX_LEN];
    char *title;
    //char artist[MAX_LEN];
    char *artist;
    int  track_count;
    struct list_head track;
}Record;

typedef struct RecordList{
    int record_cnt;
    struct list_head list;
}RecordList;


int init_record_list(void);
int add_record(char *title, char *artist, Record *record);
int add_record_wrap(char *title, char *artist, Record *record);
int add_track(Record *record, char *title, char *style, Track *track);
int add_track_wrap(char *rtitle, char *title, char *style, Track *track);
int remove_record(struct Record *record);
int remove_track(struct Record *, struct Track *);
Record *get_record_by_title(char *title);
int check_track_by_title(char *title); 
int get_track_by_title(char *title); 
Track *get_track_by_title_of_record(Record *record, char *ttitle);
int display_all_records(void);
