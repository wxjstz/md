#include <stdio.h>
#include <stdlib.h>

#include "md.h" /* this must follow `#include <stdlib.h>`  */

void __attribute__((constructor)) md_init_init()
{
    md_init();
}
void __attribute__((destructor)) md_dump_fini()
{
    md_dump(stdout);
}



int main()
{
    void *p = malloc(10);
    p = realloc(p,20);
    p = realloc(p,30);
    p = realloc(p,40);
    //free(p);
    
    p = calloc(10,2);
    p = realloc(p,40);
    p = realloc(p,50);
    p = realloc(p,60);
    //free(p);

}




