"""
	Python script that has the following predictors:
		- Static ALWAYS TAKEN Branch Predictor (Static - T): All branches are predicted to be taken
	Author: Srikar
"""

import sys

from utils.utils import get_traces

# Prediction for Static ALWAYS TAKEN Branch Predictor
def always_taken_static(trace_entries):
	print("TODO: Implement branch always static predictor")

	# TODO: Print inferences: accuracy

# Default case for switch
def default_func(trace_entries):
	print("You shouldn't be seeing this! Something bad must've happened")

# Function to calculate the accuracy of the predictor
def get_accuracy(trace_entries, trace_predictions):
	print("TODO: Calculate and print the accuracy of the predicted output")

# Alternative to switch case in python
predictor_algo_switch = {
	1: always_taken_static
}

if __name__ == "__main__":
	
	# Sanity check for CLI input
	if len(sys.argv) != 3:
		print("Incorrect arguments passed! Please check and try again")

	try:
		predictor_algo = int(sys.argv[1])
	except ValueError:
		print("Incorrect value entered for Predictor algorithm selection! Please check and try again")

	if not (1 <= predictor_algo <= 1):
		print("Incorrect value entered for predector! Please check and try again")
		exit(0)

	# Get trace entries from given filename
	filename = sys.argv[2]
	print(f"Reading traces from {filename}...")
	trace_entries = get_traces(filename)

	# Run the predictor
	branch_predictor_func = predictor_algo_switch.get(predictor_algo, default_func)
	print(f"Running the predictor {branch_predictor_func.__name__}")
	trace_predictions = branch_predictor_func(trace_entries)