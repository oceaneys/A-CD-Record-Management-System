#ifndef INCLUDE_LIST_H
#define INCLUDE_LIST_H

#define LIST_HEAD_INIT(name)  {&(name), &(name)}

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


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
    (elm)->next = NULL; 			\
    (elm)->prev = NULL; 			\
}while(0)

#define list_empty(head) ((head)->next == (head))

 
#define list_for_each_safe(pos,n,head)                         \
    for(pos = (head)->next,(n) = (pos)->next; (pos) != (head); \
            (pos) = (n), (n) = (pos)->next )

#define offsetof(type, member) (size_t)&(((type *)0)->member)

#define container_of(ptr, type, member) ({\
   const typeof(((type *)0)->member)*__mptr = ptr; \
   (type *)((char *)__mptr - offsetof(type,member));})


#endif
