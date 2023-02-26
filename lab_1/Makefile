# -*- MakeFile -*-

all: main

main: main.o utils/trace_handler.o utils/predictors.o
	gcc main.o utils/trace_handler.o utils/predictors.o -o main

main.o: utils/trace_handler.h utils/predictors.h
	gcc main.c -o main.o -c

utils/trace_handler.o: utils/trace_handler.h
	gcc utils/trace_handler.c -o utils/trace_handler.o -c

utils/predictors.o: utils/trace_handler.h
	gcc utils/predictors.c -o utils/predictors.o -c

clean:
	rm -fr main main.o utils/*.o