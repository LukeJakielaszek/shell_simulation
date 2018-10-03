CFLAGS = -g

objects = main.o llist.o parsing.o utilities.o

test : $(objects)
	cc -o test $(objects) $(CFLAGS)

main.o : llist.h parsing.h utilities.h

.PHONY : clean
clean :
	rm test $(objects)
