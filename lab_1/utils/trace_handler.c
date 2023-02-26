/*
	Module to handle all the functionality with traces`

	Author: Srikar
*/

#include <stdbool.h>
#include <stdio.h>

// Base structure that will store a trace line entry
struct trace_entry
{
	unsigned short int branch_addr;
	bool branch_taken;
};

typedef struct trace_entry trace_entry;

int get_num_traces(FILE *tracefile)
{
	int num_traces = 0;

	char c;

	// Count number of lines in the file
	while ((c = fgetc(tracefile)) != EOF)
		if (c == '\n')
			num_traces++;

	// Reset the filepointer to read from beginning for next read
	rewind(tracefile);

	return num_traces;
}