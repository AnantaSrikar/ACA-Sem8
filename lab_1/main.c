/*
	Driver code that handles user IO and calls the predictors

	Author: Srikar
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils/trace_handler.h"
#include "utils/predictors.h"

int main(int argc, char **argv)
{
	// CLI Args sanity check
	if(!(argc >= 3 && argc <= 5))
	{
		printf("Wrong number of arguments passed! Please check and try again.\n");
		return -1;
	}

	// Branch predictor choice should be an int
	for(int j = 0; j < strlen(argv[1]); j++)
	{
		if(!isdigit(argv[1][j]))
		{
			printf("Incorrect input for Predictor algorithm selection! Please check and try again.\n"); // Enter only numbers!
			return -1;
		}
	}

	int predictor_choice = atoi(argv[1]);
	unsigned int num_bits = 0;
	unsigned int hist_bits = 0;

	// No of bits used should be an int
	if(argc > 3)
	{
		for(int i = 3; i < argc; i++)
		{
			for(int j = 0; j < strlen(argv[i]); j++)
			{
				if(!isdigit(argv[i][j]))
				{
					printf("Incorrect input for No. of bits! Please check and try again.\n"); // Enter only numbers!
					return -1;
				}
			}
		}
	}

	// Predictor choice should be in valid range
	if(!(predictor_choice >= 1 && predictor_choice <= 5))
	{
		printf("Incorrect value entered for Predictor algorithm selection! Please check and try again.\n");
		return -1;
	}

	// Check if no of bits is needed, based on the predictor selected
	if(predictor_choice > 3)
	{
		if(argc != predictor_choice)	// These are coincidentally similar lol, no real logic tbh
		{
			printf("Wrong number of arguments passed! Please check and try again.\n");
			return -1;
		}

		switch(predictor_choice)
		{
			case 5:
				hist_bits = atoi(argv[4]);
			
			case 4:
				num_bits = atoi(argv[3]);
				break;
			
			default:
				printf("Something terrible must've happened, you should not be seeing this!\n");
		}
	}

	FILE *tracefile = fopen(argv[2], "r");

	// Check if the file is accessible
	if (tracefile == NULL)
	{
		printf("Entered tracefile couldn't be opened! Please check and try again.\n");
		return -1;
	}

	else
	{
		printf("Reading traces from '%s'\n", argv[2]);
	}
	
	int num_traces = get_num_traces(tracefile);

	trace_entry_t* trace_arr = get_traces(tracefile);

	// Always remember to close an opened file!
	fclose(tracefile);
	
	// Run the predictor based on user input
	trace_entry_t* pred_traces;

	printf("Running the predictor ");
	switch(predictor_choice)
	{
		case 1:
				printf("Static ALWAYS TAKEN...\n");
				pred_traces = static_always_taken(trace_arr, num_traces);
				break;

		case 2:
				printf("Static ALWAYS NOT TAKEN...\n");
				pred_traces = static_always_not_taken(trace_arr, num_traces);
				break;

		case 3:
				printf("Dynamic Last Taken...\n");
				pred_traces = dynamic_last_taken(trace_arr, num_traces);
				break;

		case 4:
				printf("Dynamic BIMODAL...\n");
				pred_traces = dynamic_bimodal(trace_arr, num_traces, num_bits);
				break;

		case 5:
				printf("Dynamic - GSHARE...\n");
				pred_traces = dynamic_gshare(trace_arr, num_traces, num_bits, hist_bits);
				break;

		default:
				printf("If you're reading this, something very terrible happend in the code! Exiting...\n");
	}

	float accuracy = get_prediction_accuracy(trace_arr, pred_traces, num_traces);
	printf("Accuracy: %f%%\n", accuracy);

	// Free the memory!!
	free(trace_arr);
	free(pred_traces);

	return 0;
}