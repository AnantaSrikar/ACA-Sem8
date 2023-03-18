# -*- MakeFile -*-

all: main

DEBUG = -g3	# Comment line to disable debugging
CFLAGS += -c
CFLAGS += $(DEBUG)

main: main.o utils/trace_handler.o utils/predictors.o
	gcc main.o utils/trace_handler.o utils/predictors.o -o main $(DEBUG)

main.o: utils/trace_handler.h utils/predictors.h main.c
	gcc main.c -o main.o $(CFLAGS)

utils/trace_handler.o: utils/trace_handler.h utils/trace_handler.c
	gcc utils/trace_handler.c -o utils/trace_handler.o $(CFLAGS)

utils/predictors.o: utils/trace_handler.h utils/predictors.c
	gcc utils/predictors.c -o utils/predictors.o $(CFLAGS)

clean:
	rm -fr main main.o utils/*.o