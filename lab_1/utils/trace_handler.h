/*
	Header file for funtions used in trace_handler.c
	Author: Srikar
*/

// Base structure that will store a trace line entry
// NOTE: Increase size of branch_addr if needed. Currently assuming 5-bit addresses as per tracefiles
typedef struct trace_entry
{
	unsigned int branch_addr;
	unsigned short int branch_taken;
} trace_entry_t;

// Function prototypes
int get_num_traces(FILE*);
trace_entry_t* new_trace_arr(int);
trace_entry_t* get_traces(FILE*);