#ifndef __LIST_H__
#define __LIST_H__

/* implement doubly linked list */


#define offsetof(type,member)	\
			((char*)(&(((type*)0)->member)) - (char*)0)

#define container_of(ptr,type,member)	\
			((type*)((char*)(ptr) - offsetof(type,member)))


typedef struct node
{
	struct node *prev,*next;
}list_head;

int  __list_empty(list_head *head);
void __list_add(list_head *prev,list_head *next,list_head *n);
void __list_remve(list_head *n);
void __list_add_prev(list_head *pos,list_head *n);
void __list_add_next(list_head *pos,list_head *n);
void __list_add_head(list_head *head,list_head *n);
void __list_add_tail(list_head *head,list_head *n);
void __list_replace(list_head *old,list_head *new);



#define DEFINE_LIST_HEAD(name)\
	list_head name={&name,&name}


#define INIT_LIST_HEAD(name)\
	(name).prev = (name).next = &(name)


#define BEGIN_LIST_EACH(iter,head,type,member)\
	do{\
		type *iter;\
		list_head *__pos,*__next;\
		for(__pos = head.next, __next = __pos->next;\
			__pos != &head;\
			__pos = __next, __next = __pos->next)\
		{\
			iter = container_of(__pos,type,member);


#define BEGIN_LIST_EACH_REVERSE(iter,head,type,member)\
	do{\
		type *iter;\
		list_head *__pos,*__prev;\
		for(__pos = head.prev, __prev = __pos->prev;\
			__pos != &head;\
			__pos = __prev, __prev = __pos->prev)\
		{\
			iter = container_of(__pos,type,member);


#define END_LIST_EACH()\
		}\
	}while(0);




#define list_empty(head)\
	__list_empty(&head)

#define list_add_prev(pos,n,member)\
	__list_add_prev(&(pos->member),&(n->member))

#define list_add_next(pos,n,member)\
	__list_add_next(&(pos->member),&(n->member))

#define list_add_head(head,n,member)\
	__list_add_head(&head,&(n->member))

#define list_add_tail(head,n,member)\
	__list_add_tail(&head,&(n->member))

#define list_replace(old,new,member)\
	__list_replace(&(old->member),&(new->member))

#define list_remove(n,member)\
	__list_remove(&(n->member))

#define list_peek_head(head,type,member)\
    container_of(head.next,type,member)

#define list_remove_head(head)\
    __list_remove(head.next)
    
#define list_peek_tail(head,type,member)\
    container_of(head.prev,type,member)

#define list_remove_tail(head)\
    __list_remove(head.prev)

#endif
