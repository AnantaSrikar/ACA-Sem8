/*
	Driver code that handles IO and calls the predictors
	Author: Srikar
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils/trace_handler.h"

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

	for(int i = 0; i < num_traces; i++)
		printf("0x%x %d\n", trace_arr[i].branch_addr, trace_arr[i].branch_taken);


	// Always remember to close an opened file!
	fclose(tracefile);

	return 0;
}