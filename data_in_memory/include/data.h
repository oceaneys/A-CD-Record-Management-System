#define MAX_LEN 256 

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


int init_record_list(void);
int add_record(struct Record *);
int add_track(struct Record *, struct Track *);
int remove_record(struct Record *record);
int remove_track(struct Record *, struct Track *);
int find_record_by_title(char *title);
int find_track_by_title(char *title); 
int display_all_records(void);

