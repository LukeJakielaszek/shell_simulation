CFLAGS = -g

objects = main.o llist.o parsing.o utilities.o

myshell : $(objects)
	cc -o myshell $(objects) $(CFLAGS)

main.o : llist.h parsing.h utilities.h

.PHONY : clean
clean :
	rm test $(objects)
