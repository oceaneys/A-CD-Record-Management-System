

/*simple doubly linked list implementation.*/

struct list_head{
    struct list_head *prev,*next;
};

#define MAX_LEN 256 

#define LIST_HEAD_INIT(name)  {&(name), &(name)}

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

typedef struct Track{
    char title[MAX_LEN];
    char style[MAX_LEN];
    struct list_head list;
}Track;

typedef struct Record{
    char title[MAX_LEN];
    char artist[MAX_LEN];
    int  track_count;
    struct list_head track;
    struct list_head list;
}Record;

typedef struct RecordList{
    int record_cnt;
    struct list_head list;
}RecordList;

#define list_add(elm,head) do { \
    (elm)->next = (head)->next;     \
    (head)->next->prev = (elm);     \
    (head)->next = (elm);           \
    (elm)->prev = (head);           \
}while(0)                       
    

#define list_add_tail(elm,head) do { \
    (elm)->prev = (head)->prev;          \
    (head)->prev->next = (elm);           \
    (elm)->next = (head);               \
    (head)->prev = (elm);               \
}while(0) 

#define list_del(elm) do {           \
    (elm)->next->prev = (elm)->prev; \
    (elm)->prev->next = (elm)->next; \
}while(0)

 
#define list_for_each_safe(pos,n,head)                         \
    for(pos = (head)->next,(n) = (pos)->next; (pos) != (head); \
            (pos) = (n), (n) = (pos)->next )

#define offsetof(type, member) (size_t)&(((type *)0)->member)

#define container_of(ptr, type, member) ({\
   const typeof(((type *)0)->member)*__mptr = ptr; \
   (type *)((char *)__mptr - offsetof(type,member));})

