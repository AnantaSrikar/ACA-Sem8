/*
	Header file for funtions used in predictors.c

	Author: Srikar
*/

float get_prediction_accuracy(trace_entry*, trace_entry*, int);
trace_entry* static_always_taken(trace_entry*, int);
trace_entry* static_always_not_taken(trace_entry*, int);