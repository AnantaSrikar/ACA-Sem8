"""
	- Script that contains all the functions used in the branch predictor algorithm
	- Trace file handling is done here
	Author: Srikar
"""

# For testing the script
import sys

# Funtion to read the file and return a (bigish) list of line entries
def get_traces(trace_filename):
	
	# List of all the trace line entries
	trace_entries = []

	try:
		# File handling of the trace file
		with open(trace_filename, 'r') as inFPtr:

			# Iterate through each line of trace entry
			for trace_line in inFPtr:

				split_line  = trace_line.split(" ")
				
				# Convert 0s and 1s to True and False resp. (Python things)
				if split_line[1] == '0':
					split_line[1] = False
				else:
					split_line[1] = True
				
				trace_entries.append(split_line)

		return trace_entries

	except FileNotFoundError:
		print("The file wasn't found! Check path and try again?")
		exit()

	# Ideally this should never happen bruv
	except Exception as e:
		print(f"Something very bad happened: {e}")
		exit()

# Testing the code

if __name__ == "__main__":

	# Sanity check for CLI input
	if len(sys.argv) < 2:
		print("Enter the file name!")
		exit()

	elif len(sys.argv) > 2:
		print("Enter only one file name!")
		exit()
	
	filename = sys.argv[1]

	# Get file name from CLI
	print(get_traces(filename))