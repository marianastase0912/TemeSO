CC = gcc
CFLAGS = -Wall -g
 
build: so-cpp
 
so-cpp: so-cpp.o hashmap.o utilsFunctions.o
	gcc $(CFLAGS) $^ -o so-cpp

so-cpp.o: so-cpp.c
	gcc $(CFLAGS) so-cpp.c -c

hashmap.o: hashmap.c hashmap.h
	gcc $(CFLAGS) hashmap.c hashmap.h -c

utilsFunctions.o: utilsFunctions.c utils.h
	gcc $(CFLAGS) utilsFunctions.c utils.h -c

.PHONY: clean
 
clean:
	rm -f *.o *~ so-cpp
