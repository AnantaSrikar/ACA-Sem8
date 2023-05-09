# Lab_1

## Running the Assignment
### Setup
To run the assignment, follow the steps below:<br>
1. Clone this repository.
2. Inside the directory `lab_1`, copy the tracefiles folder as `traces`
3. Run `make`. You will see an executable named `main`.

### Execution
The program can run the following branch predictors:
1. Static ALWAYS TAKEN Branch Predictor
2. Static ALWAYS NOT TAKEN Branch Predictor
3. Dynamic Last Taken Branch Predictor
4. Dynamic BIMODAL Branch Predictor
5. Dynamic BIMODAL Branch Predictor with Global Branch History

To run the executable, the syntax is as follows:
```./main <branch_predictor_num> <relative_tracefile_path> <additional_args>```

For example, if you want to run `Dynamic Last Taken Branch Predictor` on tracefile `traces/trace_03`, the command would be:
```./main 1 traces/trace_03`

NOTE:
- Dynamic BIMODAL Branch Predictor needs an additional argument, for the number of bits in the program counter.
- Dynamic BIMODAL Branch Predictor with Global Branch History needs two additional arguments, one for the number of bits in the program counter and another for number of bits in the history register.