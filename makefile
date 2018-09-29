CFLAGS = -g

objects = main.o llist.o parsing.o

test : $(objects)
	cc -o test $(objects)

main.o : llist.h parsing.h

.PHONY : clean
clean :
	rm test $(objects)
