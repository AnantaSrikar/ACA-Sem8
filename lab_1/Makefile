# -*- MakeFile -*-

all: main

main: main.o utils/trace_handler.o
	gcc main.o utils/trace_handler.o -o main -g3

main.o: utils/trace_handler.h
	gcc main.c -o main.o -c -g3

utils/trace_handler.o: utils/trace_handler.h
	gcc utils/trace_handler.c -o utils/trace_handler.o -c -g3

clean:
	rm -fr main main.o utils/*.o