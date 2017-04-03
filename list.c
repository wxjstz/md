#include "list.h"

/* 双向链表实现
 * list_head为双向链表的表头
 * 链表节点需要包含list_head的元素
 */

/* 判断一个双向链表是否为空 */
int __list_empty(list_head *head)
{
	return head->next == head;
}

/* 双向链表中加入节点 */
void __list_add(list_head *prev,list_head *next,list_head *n)
{
	prev->next = n;
	next->prev = n;
	n->prev = prev;
	n->next =next;
}

/* 双向链表中移除节点 */
void __list_remove(list_head *n)
{
	list_head *prev = n->prev;
	list_head *next = n->next;
	prev->next = next;
	next->prev =prev;
	n->prev = n->next = n;
}

/* 在节点pos前插入节点n */
void __list_add_prev(list_head *pos,list_head *n)
{
	__list_add(pos->prev,pos,n);
}

/* 在节点pos后插入节点n */
void __list_add_next(list_head *pos,list_head *n)
{
	__list_add(pos,pos->next,n);
}

/* 在链表头部插入节点n */
void __list_add_head(list_head *head,list_head *n)
{
	__list_add_next(head,n);
}

/* 在链表尾部插入元素n */
void __list_add_tail(list_head *head,list_head *n)
{
	__list_add_prev(head,n);
}

/* 用节点new替换节点old */
void __list_replace(list_head *old,list_head *new)
{
	list_head *prev = old->prev;
	list_head *next = old->next;
	__list_remove(old);
	__list_add(prev,next,new);
}


//#define TEST_LIST
#ifdef TEST_LIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DEFINE_LIST_HEAD(strtab);
typedef struct
{
	char* string;
	list_head node;
}strtab_t;


strtab_t* new_strtab(char *str)
{
	strtab_t *new = malloc(sizeof(*new));
	new->string = strdup(str);
	return new;
}

strtab_t* find_strtab(char *str)
{
	BEGIN_LIST_EACH(iter,strtab,strtab_t,node){
		if(strcmp(iter->string,str) == 0)
			return iter;
	}END_LIST_EACH();
	return NULL;
}

void distory_strtab()
{
	BEGIN_LIST_EACH(iter,strtab,strtab_t,node){
		list_remove(iter,node);
		free(iter->string);
		free(iter);
	}END_LIST_EACH();
}

void strtab_dump()
{
	BEGIN_LIST_EACH(iter,strtab,strtab_t,node){
		printf("%s\n",iter->string);
	}END_LIST_EACH();
	printf("\n");
}

int main()
{
	list_add_tail(strtab,new_strtab("123"),node);
	list_add_tail(strtab,new_strtab("456"),node);
	list_add_tail(strtab,new_strtab("789"),node);
	strtab_dump();
	list_add_head(strtab,new_strtab("ghi"),node);
	list_add_head(strtab,new_strtab("def"),node);
	list_add_head(strtab,new_strtab("abc"),node);
	strtab_dump();
	list_add_next(find_strtab("123"),new_strtab("234"),node);
	list_add_prev(find_strtab("def"),new_strtab("cde"),node);
	strtab_dump();
	distory_strtab();
	if(list_empty(strtab))
		printf("strtab is empty\n");
	return 0;
}

#endif










