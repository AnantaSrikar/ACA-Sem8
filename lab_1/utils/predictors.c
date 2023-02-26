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

// Static ALWAYS TAKEN Branch Predictor
trace_entry* static_always_taken(trace_entry* trace_arr, int num_traces)
{
	// New trace_entry array to store all the predictions
	trace_entry* trace_predictions = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;
		trace_predictions[i].branch_taken = 1;	// Branch will always be taken
	}

	return trace_predictions;
}

// Static ALWAYS NOT TAKEN Branch Predictor
trace_entry* static_always_not_taken(trace_entry* trace_arr, int num_traces)
{
	// New trace_entry array to store all the predictions
	trace_entry* trace_predictions = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;
		trace_predictions[i].branch_taken = 0;	// Branch will always NOT be taken
	}

	return trace_predictions;
}

// Dynamic Last Taken Branch Predictor
trace_entry* dynamic_last_taken(trace_entry* trace_arr, int num_traces)
{
	// New trace_entry array to store all the predictions
	trace_entry* trace_predictions = new_trace_arr(num_traces);

	// New trace_entry array to keep track of all branches
	// NOTE: It is not space or time optimised. This is scope for performace improments
	trace_entry* trace_track = new_trace_arr(num_traces);
	int trace_count = 0;

	for(int i = 0; i < num_traces; i++)
	{
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;

		// Check if the branch has been encountered in the past
		for(int j = 0; j < num_traces; j++)
			// If the entry exists
			if(trace_arr[i].branch_addr == trace_track[j].branch_addr)
			{
				trace_predictions[i].branch_taken = trace_track[j].branch_taken;

				// Update the last entry with current entry if they are different
				if(trace_track[j].branch_taken != trace_arr[i].branch_taken)
					trace_track[j].branch_taken = trace_arr[i].branch_taken;

				break;
			}

			else
			{
				trace_track[trace_count].branch_addr = trace_arr[i].branch_addr;
				trace_predictions[i].branch_taken = 1;	// Branch is assumed to be taken by default

				// Update entry with current branch decision
				trace_track[trace_count].branch_taken = trace_arr[i].branch_taken;
			}
		
	}

	return trace_predictions;
}