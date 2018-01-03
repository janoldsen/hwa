CC=gcc
CFLAGS = I.

hwa.o : hwa.h hwa.c
	$(CC) -c -g hwa.c

main_test.o : hwa.h main_test.o
	$(CC) -c -g main_test.c

test : main_test.o hwa.o
	$(CC) -o test main_test.o hwa.o

.PHONY : clean

clean :
	-rm -f *.o test.exe
