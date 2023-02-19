"""
	- Python script to analyze a given trace file
	- Find the total number of trace entires and number of unique branch instructions
	- Format of the traces: <branch_instruction_addr> <branch_result>. Eg: 0x40fc96 1

	Author: Srikar
"""

BASE_FILENAME = "traces/trace_"

# Iterate through all tracefiles
for file_num in range(1, 17):

	# List to store all the addresses of the branch instructions
	branch_addrs = []

	# To keep track of number of entries
	line_cnt = 0

	# Open the tracefile
	with open(f"{BASE_FILENAME}{file_num:02d}", 'r') as inFPtr:

		# Iterate through each line
		for count, line in enumerate(inFPtr):

			split_line = line.split(" ")
			branch_addr = split_line[0]
			result = split_line[1]

			# Making a list of all unique branch addresses
			if branch_addr not in branch_addrs:
				branch_addrs.append(branch_addr)

		line_cnt = count

	print(f"Analysis of {BASE_FILENAME}{file_num:02d}:")
	print(f"No of Trace entries: {line_cnt}")
	print(f"No of Branches: {len(branch_addrs)}\n")