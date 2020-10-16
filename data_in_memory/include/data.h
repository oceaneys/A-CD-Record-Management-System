#define MAX_LEN 256 

/*simple doubly linked list implementation.*/

char current_cd[MAX_LEN];

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


void add_record_ui();
void remove_record_ui();
void find_record_ui();
void list_record_ui();
void diaplay_all_records_ui();

int add_record(Record *record);
int add_record_wrap(char *title, char *artist, Record *record);
int remove_record(struct Record *record);
Record *get_record_by_title(char *title);
int display_all_records(void);

int add_track(Record *record, Track *track);
int add_track_wrap(char *rtitle, char *title, char *style);
int remove_track(struct Record *, struct Track *);
int check_track_by_title(char *title); 
int get_track_by_title(char *title); 
Track *get_track_by_title_of_record(Record *record, char *ttitle);


void get_return();
void clear_all_screen();


