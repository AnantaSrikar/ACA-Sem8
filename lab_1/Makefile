# -*- MakeFile -*-

all: main

main: main.o utils/trace_handler.o
	gcc main.o utils/trace_handler.o -o main

main.o: utils/trace_handler.h
	gcc main.c -o main.o -c

utils/trace_handler.o:
	gcc utils/trace_handler.c -o utils/trace_handler.o -c

clean:
	rm -fr main main.o utils/*.o