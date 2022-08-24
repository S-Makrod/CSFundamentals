main : procsub.o sample-main.o
	gcc -g $^ -o $@

%.o : %.c
	gcc -g -c $<

clean :
	rm -f *.o main .depend
.PHONY: clean

.depend: procsub.c sample-main.c
	gcc -MM $^ > .depend
include .depend
