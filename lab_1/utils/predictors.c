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
#include <stdint.h>

#include "trace_handler.h"

// TODO: Update comments for functions

// Returns the accuracy of the predictions compared to the tracefile
float get_prediction_accuracy(trace_entry_t* file_traces, trace_entry_t* pred_traces, int num_traces)
{
	int good_preds = 0;

	for(int i = 0; i < num_traces; i++)
	{
		if(file_traces[i].branch_taken == pred_traces[i].branch_taken)
		{
			good_preds++;
		}
	}

	return ((float)good_preds / num_traces) * 100;
}

// Static ALWAYS TAKEN Branch Predictor
trace_entry_t* static_always_taken(trace_entry_t* trace_arr, int num_traces)
{
	// New trace_entry_t array to store all the predictions
	trace_entry_t* trace_predictions = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;
		trace_predictions[i].branch_taken = 1;	// Branch will always be taken
	}

	return trace_predictions;
}

// Static ALWAYS NOT TAKEN Branch Predictor
trace_entry_t* static_always_not_taken(trace_entry_t* trace_arr, int num_traces)
{
	// New trace_entry_t array to store all the predictions
	trace_entry_t* trace_predictions = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;
		trace_predictions[i].branch_taken = 0;	// Branch will always NOT be taken
	}

	return trace_predictions;
}

// Function to calculate the number of unique branch addresses
int get_num_unique_branches(trace_entry_t* trace_arr, int num_traces, uint32_t addr_mask)
{
	int num_unique_branches = 0;
	trace_entry_t* trace_track = new_trace_arr(num_traces);

	for(int i = 0; i < num_traces; i++)
	{
		// Checking if the current branch address is already encountered
		uint32_t masked_branch_addr = trace_arr[i].branch_addr & addr_mask;

		// First element is always not found, hence append
		if(i == 0)
		{
			trace_track[num_unique_branches].branch_addr = masked_branch_addr;
			trace_track[num_unique_branches].branch_taken = 1;	// Since default branch taken is 1
			num_unique_branches++;

			continue;
		}

		int found = 0;
		// For all other entries, run loop to check if branch address is already there in our list of unique entries
		for(int j = 0; j < num_unique_branches; j++)
		{
			if(trace_track[j].branch_addr == masked_branch_addr)
			{
				found = 1;
				break;
			}
		}

		// Append element to list of unique branch adresses 
		if(!found)
		{
			trace_track[num_unique_branches].branch_addr = masked_branch_addr;
			num_unique_branches++;
		}
	}

	// Always free allocated memory after using it
	free(trace_track);

	return num_unique_branches;
}

// Dynamic Last Taken Branch Predictor
trace_entry_t* dynamic_last_taken(trace_entry_t* trace_arr, int num_traces)
{
	uint32_t addr_mask = 0;
	// Generate mask with all 1s for compatibility of checking unique traces function
	for(int i = 0; i < (sizeof(uint32_t) * 8); i++)
	{
		addr_mask |= (1 << i);
	}

	int num_unique_branches = get_num_unique_branches(trace_arr, num_traces, addr_mask);
	// New trace_entry_t array to store all the predictions
	trace_entry_t* trace_predictions = new_trace_arr(num_traces);

	// New trace_entry_t array to keep track of all branches
	// NOTE: It is not time optimised. This is scope for performace improvements
	trace_entry_t* unique_branchs = new_trace_arr(num_unique_branches);
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
				{
					unique_branchs[j].branch_taken = trace_arr[i].branch_taken;
				}

				found = 1;
				break;
			}
		}

		// if we're encountering the branch address for the first time
		if(!found)
		{
			unique_branchs[trace_count].branch_addr = trace_arr[i].branch_addr;
			trace_predictions[i].branch_taken = 1;	// Branch is assumed to be taken by default

			// Update entry with current branch decision
			unique_branchs[trace_count].branch_taken = trace_arr[i].branch_taken;
			trace_count++;
		}
	}

	// Again, don't forget to free the memory!
	free(unique_branchs);

	return trace_predictions;
}


// Dynamic BIMODAL Branch Predictor
trace_entry_t* dynamic_bimodal(trace_entry_t* trace_arr, int num_traces, int n)
{
	// Generate the mask
	unsigned int addr_mask = 0;
	for(int i = 0; i < n; i++)
	{
		addr_mask |= (1 << i);
	}

	// Get all unique branches with address mask
	int num_unique_branches = get_num_unique_branches(trace_arr, num_traces, addr_mask);
	// New trace_entry_t array to store all the predictions
	trace_entry_t* trace_predictions = new_trace_arr(num_traces);
	

	// New trace_entry_t array to keep track of all branches
	// NOTE: It is not time optimised. This is scope for performace improvements
	trace_entry_t* unique_branchs = new_trace_arr(num_unique_branches);		// We will store the 2-bit saturating counter in branch_taken, varying values from 0-3
	int trace_count = 0;
	
	// Start the predictions!
	for(int i = 0; i < num_traces; i++)
	{
		int found = 0;

		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;

		// Check if the branch has been encountered in the past
		for(int j = 0; j < num_unique_branches; j++)
		{
			// If the entry (of first n bits of address) exists
			if((trace_arr[i].branch_addr & addr_mask) == unique_branchs[j].branch_addr)
			{
				trace_predictions[i].branch_taken = unique_branchs[j].branch_taken >= 2 ? 1 : 0;		// 0 - SNT, 1 - WNT, 2 - WT, 3 - ST. Take the branch if pred > 1

				// Update the counter based on current entry
				if(trace_arr[i].branch_taken)
				{
					if(unique_branchs[j].branch_taken < 3)
					{
						unique_branchs[j].branch_taken++;
					}
				}

				else
				{
					if(unique_branchs[j].branch_taken > 0)
					{
						unique_branchs[j].branch_taken--;
					}
				}

				found = 1;
				break;
			}
		}

		// if we're encountering the branch address for the first time
		if(!found)
		{
			unique_branchs[trace_count].branch_addr = trace_arr[i].branch_addr & addr_mask;
			trace_predictions[i].branch_taken = 1;	// Branch is assumed to be taken by default
			unique_branchs[trace_count].branch_taken = 3;		// Assuming every branch is strongly taken

			trace_count++;
		}
	}

	// Again, don't forget to free the memory!
	free(unique_branchs);

	return trace_predictions;
}


/**
 * @brief Dynamic BIMODAL Branch Predictor with Global Branch History
 * 
 * @param trace_arr raw list of all trace addresses with its path value
 * @param num_traces Number of trace entries in the above list
 * @param n Number of bits to be masked in the address
 * @param h Number of last h branches to remember
 * @return trace_entry_t* predicted trace_entry array
 */
trace_entry_t* dynamic_gshare(trace_entry_t* trace_arr, int num_traces, int n, int h)
{
	// Generate the mask
	unsigned int addr_mask = 0;
	for(int i = 0; i < n; i++)
	{
		addr_mask |= (1 << i);
	}

	// Get all unique branches with address mask
	int num_unique_branches = get_num_unique_branches(trace_arr, num_traces, addr_mask);

	// New trace_entry_t array to store all the predictions
	trace_entry_t* trace_predictions = new_trace_arr(num_traces);
	
	// New trace_entry_t array to keep track of all branches
	// NOTE: It is not time optimised. This is scope for performace improvements
	trace_entry_t* unique_branchs = new_trace_arr(num_unique_branches);		// We will store the 2-bit saturating counter in branch_taken, varying values from 0-3
	int trace_count = 0;

	// History tracking
	trace_entry_t* global_history = new_trace_arr(h);
	int global_history_index = 0;
	
	// Start the predictions!
	for(int i = 0; i < num_traces; i++)
	{
		int found = 0;
		
		trace_predictions[i].branch_addr = trace_arr[i].branch_addr;

		// Check if the branch has been encountered in the past
		for(int j = 0; j < num_unique_branches; j++)
		{
			// If the entry (of first n bits of address) exists
			if((trace_arr[i].branch_addr & addr_mask) == unique_branchs[j].branch_addr)
			{
				int bimodal_prediction = unique_branchs[j].branch_taken >= 2 ? 1 : 0;		// 0 - SNT, 1 - WNT, 2 - WT, 3 - ST. Take the branch if pred > 1

				// The XOR time
				int global_history_sum = 0;
				for (int i = 0; i < h; i++)
				{
					global_history_sum += global_history[i].branch_addr * (1 << i);
				}

				int global_index = global_history_sum % num_unique_branches;
				int global_prediction = unique_branchs[global_index].branch_taken;

				trace_predictions[i].branch_taken = global_prediction >= 2;

				// Update the counter based on current entry
				if(trace_arr[i].branch_taken)
				{
					if(unique_branchs[j].branch_taken < 3)
					{
						unique_branchs[j].branch_taken++;
					}
				}

				else
				{
					if(unique_branchs[j].branch_taken > 0)
					{
						unique_branchs[j].branch_taken--;
					}
				}

				// Update global history
				unique_branchs[global_history_index].branch_taken = trace_predictions[i].branch_taken;
				global_history_index = (global_history_index + 1) % h;

				found = 1;
				break;
			}
		}

		// if we're encountering the branch address for the first time
		if(!found)
		{
			unique_branchs[trace_count].branch_addr = trace_arr[i].branch_addr & addr_mask;
			trace_predictions[i].branch_taken = 1;	// Branch is assumed to be taken by default
			unique_branchs[trace_count].branch_taken = 3;		// Assuming every branch is strongly taken

			trace_count++;
		}
	}

	// Again, don't forget to free the memory!
	free(unique_branchs);

	return trace_predictions;
}

// NOTE TO SELF: If h = 0, it should give same accuracy as dynamic Bimodal