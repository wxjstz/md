#ifndef __MD_H__
#define __MD_H__

void  md_init();
void* md_malloc(size_t size,char *file,size_t line);
void* md_calloc(size_t nmemb, size_t size,char *file,size_t line);
void* md_realloc(void *ptr, size_t size,char *file,size_t line);
void  md_free(void *ptr,char *file,size_t line);
void  md_dump(FILE *fp);


#ifdef DISABLE_MD

#define md_init(...)
#define md_dump(...)

#else

#define malloc(size)        md_malloc(size,__FILE__,__LINE__)
#define calloc(nmemb,size)  md_calloc(nmemb,size,__FILE__,__LINE__)
#define realloc(ptr,size)   md_realloc(ptr,size,__FILE__,__LINE__)
#define free(ptr)           md_free(ptr,__FILE__,__LINE__)

#endif

#endif
