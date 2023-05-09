/**
 * @brief Header file for funtions used in predictors.c
 * @author: Srikar
*/

float get_prediction_accuracy(trace_entry_t*, trace_entry_t*, int);
trace_entry_t* static_always_taken(trace_entry_t*, int);
trace_entry_t* static_always_not_taken(trace_entry_t*, int);
trace_entry_t* dynamic_last_taken(trace_entry_t*, int);
trace_entry_t* dynamic_bimodal(trace_entry_t*, int, int);
trace_entry_t* dynamic_gshare(trace_entry_t*, int, int, int);