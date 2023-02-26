/*
	Module to handle all the functionality with traces`

	Author: Srikar
*/

#include <stdio.h>
#include <stdlib.h>

#include "trace_handler.h"

// Get the number of trace entries for a given tracefile
int get_num_traces(FILE* tracefile)
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

// Returns a new array of trace entries for a given size
trace_entry* new_trace_arr(int num_traces)
{
	trace_entry* trace_arr = (trace_entry*)malloc(num_traces * sizeof(trace_entry));
	return trace_arr;
}

// Returns an array of all the trace entries from a given tracefile
trace_entry* get_traces(FILE* tracefile)
{
	trace_entry* trace_arr = new_trace_arr(get_num_traces(tracefile));

	int i = 0;

	// Input all the trace entries from file into trace_arr
	while(!feof(tracefile))
	{
		fscanf(tracefile, "%x %d", &trace_arr[i].branch_addr, &trace_arr[i].branch_taken);
		i++;
	}

	return trace_arr;
}