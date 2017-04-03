libmd.a : list.c list.h md.c md.h CriticalSection.h
	@gcc -c list.c
	@gcc -c md.c
	@ar rcs libmd.a md.o list.o

example:example.c libmd.a
	@gcc example.c -L. -lmd -o example

test : example
	@./example

clean :
	-@rm -f *.o *.a example
