#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "CriticalSection.h"

static CS lock;

enum{md_log_invalid,md_log_malloc,md_log_calloc,md_log_realloc};

typedef struct
{
	
	int line;
	char* file;
	
	void *p;
	int  type;
	size_t size;
	size_t nmemb;
	
	list_head node;
}md_log;

typedef struct
{
	void *p;
	list_head log;
	list_head node;
}md_list;


DEFINE_LIST_HEAD(list);

md_log* alloc_md_log(void *p,int type,size_t nmemb,size_t size,
                        char *file,int line)
{
	md_log *r = malloc(sizeof(*r));
	if(!r)
		return NULL;
	r->type = type;
	r->p = p;
	r->nmemb = nmemb;
	r->size = size;
	r->file = file;
	r->line = line;
	INIT_LIST_HEAD(r->node);
}

md_list* alloc_md_list(void *p)
{
	md_list *r = malloc(sizeof(*r));
	if(!r)
		return NULL;
	r->p = p;
	INIT_LIST_HEAD(r->log);
	INIT_LIST_HEAD(r->node);
}

md_list* search_md(void *p)
{
	BEGIN_LIST_EACH(iter,list,md_list,node)
	{
		if(iter->p >= p)
			return iter;
	}
	END_LIST_EACH();
	return NULL;
}

void md_init()
{
    CS_INIT(&lock);
}
void* md_malloc(size_t size,char *file,size_t line)
{
	void *p = malloc(size);
	if(!p)
		return p;

	CS_ENTRY(&lock);
	md_list *pos = search_md(p);
	if((pos == NULL) || (pos->p > p))
	{
		md_list *new = alloc_md_list(p);
		if(pos)
			list_add_prev(pos,new,node);
		else
			list_add_tail(list,new,node);
		pos = new;	
	}
	md_log *log = alloc_md_log(p,md_log_malloc,1,size,file,line);
	list_add_tail(pos->log,log,node);
	CS_LEAVE(&lock);
	
	return p;
}

void* md_calloc(size_t nmemb, size_t size,char *file,size_t line)
{
    void *p = calloc(nmemb,size);
	if(!p)
		return p;
	
	CS_ENTRY(&lock);
	md_list *pos = search_md(p);
	if((pos == NULL) || (pos->p > p))
	{
		md_list *new = alloc_md_list(p);
		if(pos)
			list_add_prev(pos,new,node);
		else
			list_add_tail(list,new,node);
		pos = new;	
	}
	md_log *log = alloc_md_log(p,md_log_calloc,nmemb,size,file,line);
	list_add_tail(pos->log,log,node);
	CS_LEAVE(&lock);
	
	return p;
}


void* md_realloc(void *ptr, size_t size,char *file,size_t line)
{
    void *p = realloc(ptr,size);
	if(!p)
		return p;
	
	CS_ENTRY(&lock);
	md_list *pos = search_md(ptr);
	if(pos == NULL || pos->p != ptr)
	{
	    assert(0);
	}
	pos->p = p;
	
	md_log *log = alloc_md_log(p,md_log_realloc,1,size,file,line);
	list_add_tail(pos->log,log,node);
	CS_LEAVE(&lock);
	
	return p;
}



void md_free(void *ptr,char *file,size_t line)
{
    CS_ENTRY(&lock);
    md_list *pos = search_md(ptr);
	if(pos == NULL || pos->p != ptr)
	{
	    assert(0);
	}
	
	while(!list_empty(pos->log))
	{
	    md_log *log = list_peek_head(pos->log,md_log,node);
	    list_remove_head(pos->log);
	    free(log);
	}
	
	list_remove(pos,node);
	free(pos);
	CS_LEAVE(&lock);
	
	free(ptr);
}

void md_dump()
{
    while(!list_empty(list))
    {
        md_list* md = list_peek_head(list,md_list,node);
        list_remove_head(list);
        printf("%p : \n",md->p);
        while(!list_empty(md->log))
        {
            md_log* log = list_peek_head(md->log,md_log,node);
            list_remove_head(md->log);
            switch(log->type)
            {
                case md_log_malloc:
                    printf("\t%p malloc %lu bytes in [%s : %d]\n",
                        log->p,log->size,log->file,log->line);
                    break;
                case md_log_calloc:
                    printf("\t%p calloc %lu * %lu bytes in [%s : %d]\n",
                        log->p,log->nmemb,log->size,log->file,log->line);
                    break;
                case md_log_realloc:
                    printf("\t%p realloc %lu bytes in [%s : %d]\n",
                        log->p,log->size,log->file,log->line);
                    break;
            }
            free(log);
        }
        free(md);
    }
}

/* issuse : don't free memory
void md_dump()
{
    BEGIN_LIST_EACH(i,list,md_list,node)
    {
        printf("%p : \n",i->p);
        BEGIN_LIST_EACH(j,i->log,md_log,node)
        {
            switch(j->type)
            {
                case md_log_malloc:
                    printf("\t%p malloc %lu bytes in [%s : %d]\n",
                        j->p,j->size,j->file,j->line);
                    break;
                case md_log_calloc:
                    printf("\t%p calloc %lu * %lu bytes in [%s : %d]\n",
                        j->p,j->nmemb,j->size,j->file,j->line);
                    break;
                case md_log_realloc:
                    printf("\t%p realloc %lu bytes in [%s : %d]\n",
                        j->p,j->size,j->file,j->line);
                    break;
            }
        }
        END_LIST_EACH();
    }
    END_LIST_EACH();
}
*/







