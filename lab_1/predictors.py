"""
	Python script that has the following predictors:
		- Static ALWAYS TAKEN Branch Predictor (Static - T): All branches are predicted to be taken
		- Static ALWAYS NOT TAKEN Branch Predictor (Static - NT): All branches are predicted to be not taken
		- Dynamic Last Taken Branch Predictor (Dynamic-LT): A branch is predicted as taken if it was taken in its last dynamic instance, and vice versa
		- Dynamic BIMODAL Branch Predictor (Dynamic-BM):
		- Dynamic BIMODAL Branch Predictor with Global Branch History (Dynamic-GSHARE)
	Author: Srikar
"""

import sys
from copy import deepcopy

from utils.utils import get_traces

# Prediction for Static ALWAYS TAKEN Branch Predictor
def static_always_taken(trace_entries):

	# Copy the trace_entries
	trace_predictions = deepcopy(trace_entries)
	
	# All the predictions will be True since all branches are always taken
	for trace_prediction in trace_predictions:
		trace_prediction[1] = True

	return trace_predictions

# Prediction for Static ALWAYS NOT TAKEN Branch Predictor
def static_always_not_taken(trace_entries):

	# Copy the trace_entries
	trace_predictions = deepcopy(trace_entries)
	
	# All the predictions will be False since all branches are always taken
	for trace_prediction in trace_predictions:
		trace_prediction[1] = False

	return trace_predictions

# Prediction for Dynamic Last Taken Branch Predictor
def dynamic_last_taken(trace_entries):

	print("NOTE: This may take a while depending on the computers specs. You could grab a coffee while you wait...")

	trace_predictions = []
	
	# Dictorionay that stores previous path for each trace
	prev_traces_path = {}

	# Iterate through each trace entry
	for trace_entry in trace_entries:

		# print(trace_entry)

		branch_path_addr = trace_entry[0]

		# Add trace to track if there is not prev entry.
		# NOTE: Default assumption is that trace is taken
		if branch_path_addr not in trace_entries:
			prev_traces_path[branch_path_addr] = trace_entry[1]
			trace_predictions.append([branch_path_addr, True])
			continue

		# Prediction will be the last state of the branch
		trace_prediction.append([branch_path_addr, prev_traces_path[branch_path_addr]])

		# Update the previous entry if current entry is different
		if prev_traces_path[branch_path_addr] != trace_entry[1]:
			prev_traces_path[branch_path_addr] = trace_entry[1]

	return trace_predictions

# Default case for switch
def default_func(trace_entries):
	print("You shouldn't be seeing this! Something bad must've happened")

# Function to calculate the accuracy of the predictor
def get_accuracy(trace_entries, trace_predictions):
	
	# Find total number of incorrect predictions
	bad_preds = 0

	for i in range(len(trace_entries)):
		if trace_entries[i][1] != trace_predictions[i][1]:
			bad_preds += 1

	accuracy = (len(trace_entries) - bad_preds) / len(trace_entries) * 100

	print(f"Accuracy: {accuracy}%")

# Alternative to switch case in python
predictor_algo_switch = {
	1: static_always_taken,
	2: static_always_not_taken,
	3: dynamic_last_taken
}

if __name__ == "__main__":
	
	# Sanity check for CLI input
	if len(sys.argv) != 3:
		print("Incorrect arguments passed! Please check and try again")

	try:
		predictor_algo = int(sys.argv[1])
	except ValueError:
		print("Incorrect value entered for Predictor algorithm selection! Please check and try again")

	if not (1 <= predictor_algo <= 3):
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

	# Find the accuracy
	get_accuracy(trace_entries, trace_predictions)


# Note to self: ALWAYS deepcopy lists to save few hours of debugging