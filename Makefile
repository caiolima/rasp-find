.c.o:   $*.h
	gcc -c $*.c

.cpp.o:	$*.h
	g++	-c $*.cpp

all:	rasp-find

rasp-find:  network_utils.o main.o
	g++ -o $@ $^

clean:
	rm *.o *.*~ rasp-find 

