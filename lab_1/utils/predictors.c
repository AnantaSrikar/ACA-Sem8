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
#include <stdlib.h>

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

// Function to calculate the number of unique branch addresses
int get_num_unique_branches(trace_entry* trace_arr, int num_traces)
{
	int num_unique_branches = 0;
	trace_entry* trace_track = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		// Checking if the current branch address is already encountered

		// First element is always not found, hence append
		if(i == 0)
		{
			trace_track[num_unique_branches].branch_addr = trace_arr[i].branch_addr;
			trace_track[num_unique_branches].branch_taken = 1;	// Since default branch taken is 1
			num_unique_branches++;

			continue;
		}

		int found = 0;
		// For all other entries, check if branch address is already there in our list of unique entries
		for(int j = 0; j < num_unique_branches; j++)
		{
			if(trace_track[j].branch_addr == trace_arr[i].branch_addr)
			{
				found = 1;
				break;
			}
		}

		// Append element to list of unique branch adresses 
		if(!found)
		{
			trace_track[num_unique_branches].branch_addr = trace_arr[i].branch_addr;
			num_unique_branches++;
		}
	}

	// Always free allocated memory after using it
	free(trace_track);

	return num_unique_branches;
}

// Dynamic Last Taken Branch Predictor
trace_entry* dynamic_last_taken(trace_entry* trace_arr, int num_traces)
{
	int num_unique_branches = get_num_unique_branches(trace_arr, num_traces);
	// New trace_entry array to store all the predictions
	trace_entry* trace_predictions = new_trace_arr(num_traces);

	// New trace_entry array to keep track of all branches
	// NOTE: It is not space or time optimised. This is scope for performace improments
	trace_entry* unique_branchs = new_trace_arr(num_unique_branches);
	int trace_count = 0;
	

	// Start the predictions!
	for(int i = 0; i < num_traces; i++)
	{
		int found = 0;

		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;

		// Check if the branch has been encountered in the past
		for(int j = 0; j < num_unique_branches; j++)
		{
			// If the entry exists
			if(trace_arr[i].branch_addr == unique_branchs[j].branch_addr)
			{
				trace_predictions[i].branch_taken = unique_branchs[j].branch_taken;

				// Update the last entry with current entry if they are different
				if(unique_branchs[j].branch_taken != trace_arr[i].branch_taken)
					unique_branchs[j].branch_taken = trace_arr[i].branch_taken;

				found = 1;
				break;
			}
		}

		// if we're encountering the branch address for the first time
		if(!found)
		{
			unique_branchs[trace_count].branch_addr = trace_arr[i].branch_addr;
			trace_predictions[i].branch_taken = unique_branchs[trace_count].branch_taken = 1;	// Branch is assumed to be taken by default

			// Update entry with current branch decision
			unique_branchs[trace_count].branch_taken = trace_arr[i].branch_taken;
			trace_count++;
		}
	}

	// Again, don't forget to free the memory!
	free(unique_branchs);

	return trace_predictions;
}