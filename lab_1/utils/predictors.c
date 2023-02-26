/*
	Program that has the following predictors:
		- Static ALWAYS TAKEN Branch Predictor (Static - T): All branches are predicted to be taken
		- Static ALWAYS NOT TAKEN Branch Predictor (Static - NT): All branches are predicted to be not taken
		- Dynamic Last Taken Branch Predictor (Dynamic - LT): A branch is predicted as taken if it was taken in its last dynamic instance, and vice versa
		- Dynamic BIMODAL Branch Predictor (Dynamic - BM):
		- Dynamic BIMODAL Branch Predictor with Global Branch History (Dynamic-GSHARE)
	Author: Srikar
*/

#include <stdio.h>

#include "trace_handler.h"


// Returns the accuracy of the predictions compared to the tracefile
float get_prediction_accuracy(trace_entry* file_traces, trace_entry* pred_traces, int num_traces)
{
	int good_preds = 0;

	for(int i = 0; i < num_traces; i++)
		if(file_traces[i].branch_taken == pred_traces[i].branch_taken)
			good_preds++;

	return ((float)good_preds / num_traces) * 100;
}

trace_entry* static_always_taken(trace_entry* trace_arr, int num_traces)
{
	// New trace_entry array to store all the predictions
	trace_entry* trace_preditions = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		trace_preditions[i].branch_addr = trace_arr[i].branch_addr;
		trace_preditions[i].branch_taken = 1;	// Branch will always be taken
	}

	return trace_preditions;
}