/*
	Driver code that handles IO and calls the predictors
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
	if(argc != 3)
	{
		printf("Wrong number of arguments passed! Please check and try again.\n");
		return -1;
	}

	// Branch predictor choice should be an int
	for(int j = 0; j < strlen(argv[1]); j++)
		if(!isdigit(argv[1][j]))
		{
			printf("Incorrect input for Predictor algorithm selection! Please check and try again.\n"); // Enter only numbers!
			return -1;
		}

	int predictor_choice = atoi(argv[1]);

	// Predictor choice should be in valid range
	if(!(predictor_choice >= 1 && predictor_choice <= 5))
	{
		printf("Incorrect value entered for Predictor algorithm selection! Please check and try again.\n");
		return -1;
	}

	FILE *tracefile = fopen(argv[2], "r");

	// Check if the file is accessible
	if (tracefile == NULL)
	{
		printf("Entered tracefile couldn't be opened! Please check and try again.\n");
		return -1;
	}
	
	int num_traces = get_num_traces(tracefile);

	trace_entry* trace_arr = get_traces(tracefile);

	// Always remember to close an opened file!
	fclose(tracefile);
	
	// Run the predictor based on user input
	trace_entry* pred_traces;

	printf("Running the predictor ");
	switch(predictor_choice)
	{
		case 1:
				pred_traces = static_always_taken(trace_arr, num_traces);
				break;

		case 2:
				printf("Static ALWAYS NOT TAKEN...\n");
				break;

		case 3:
				printf("Dynamic Last Taken...\n");
				break;

		case 4:
				printf("Dynamic BIMODAL...\n");
				break;

		case 5:
				printf("Dynamic - GSHARE...\n");
				break;

		default:
				printf("If you're reading this, something very terrible happend in the code! Exiting...\n");
	}

	float accuracy = get_prediction_accuracy(trace_arr, pred_traces, num_traces);
	printf("Accuracy: %f%%\n", accuracy);

	return 0;
}