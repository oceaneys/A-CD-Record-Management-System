#ifndef INCLUDE_METHODS_H
#define INCLUDE_METHODS_H 

#define MAX_LEN 256

char current_cd[MAX_LEN];

typedef enum Bool 
{
	False = 0,
	True = 1

}Bool;

typedef struct Track{
	char rtitle[MAX_LEN];
	int  track_no;
    char title[MAX_LEN];
    char style[MAX_LEN];
}Track;

typedef struct Record{
    char title[MAX_LEN];
    char artist[MAX_LEN];
	int  track_count;
}Record;

int add_record_wrap(char *, char *);
int remove_record_wrap(char *);
void display_all_records(int, int);
Bool record_exsits(char *);

int list_track_by_title_of_record(int, int, char *);
int add_track_wrap(char *title, char *style, int track_no);
int remove_all_tracks_of_one_record(char *rtitle);

void zero_current_cd(); /* current_cd[0] = '\0' */
void fulfill_current_cd(char *);
char *get_current_cd();
char get_current_cd0();

#endif
