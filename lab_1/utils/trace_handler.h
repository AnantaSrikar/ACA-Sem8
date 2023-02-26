/*
	Header file for funtions used in trace_handler.c
	Author: Srikar
*/

// Base structure that will store a trace line entry
struct trace_entry
{
	unsigned int branch_addr;
	unsigned short int branch_taken;
};

typedef struct trace_entry trace_entry;

// Function prototypes
int get_num_traces(FILE*);
trace_entry* get_traces(FILE*);