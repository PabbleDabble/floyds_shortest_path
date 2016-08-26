# floyds_shortest_path
C++ Program, input txt file with size (length) of square matrix of weighted paths, and optional START END values

Example 3 x 3 matrix
      A     B     C
A     0     2     3
B     1     0 99999
C 99999     5     0

A conntects to B (weight 2), C (weight 3)
B conntects to A (weight 1)
C conntects to B (weight 5)
C DOES NOT connect to A (99999 = infinity)
B DOES NOT connect to C (99999 = infinity)
The weight for traveling from node 'i' to node 'i' should always be zero.
(All the values along the diagonal should be equal to zero.)
Such as --main 3x3_graph.txt--
--You can also submit the matrix text file, and a starting and ending point
To get the shortest path for that specific combo as well.
Such as --main 3x3_graph.txt 0 2-- to traval from A to C in the previously mentioned matrix.
*This is in the program's instructions / error input message also.  


Makefile has all the test case command line executions in it also

Code is thoroughly commented, but here is quick desc of functions contained within

main -- This will give instructions for bad inputs, accept 1 input (file) and 3 inputs (file, start, end)
class result_holder -- This allows us to pass back 2 arrays from floyds to main
INFINITY -- global const for infinity
NO_PATH -- global string const for what to print if there is no path


//This just prints the board, never really 'need' this, except for debugging
void print_board(int *arr, int x, int y);
//This will print strings, same as above, but for strings (the directions/paths)
void print_strings(string *arr, int x, int y);
//This will print the paths for all of them, which also isn't really needed
//This prints them in english (from/to)
void print_paths(string *arr, int x, int y);
//This will return a results case from Floyds (shortest pairs, and the directions/paths) to main
result_holder floyds_algorithm(int *arr, int x, int y);
//This checks the shortest_pairs matrix for negative cylces
bool find_neg_cycle(int *arr, int x, int y);
//Pre-check the matrix diagonals, this just checks if maxtrix(i,i) == 0, and fixes them
bool check_diagonal_zeros(int *arr, int x, int y);
//This will separate the path into individual values, 
//It was easier for me to build the directions just concatenating a string, then deparse after
//Instead of vectors or some push/pop misery
int * deparser(string path, int max_size);



Case Filename		Comment
8_all_zeroes.txt	Doesn't do anything, thinks all nodes are connected by one edge
8_all_inf.txt		Fixed main.cpp to "Error" on nodes with no shortest paths
3_graph.txt		Works
4_bad_diagonals.txt	Asks to fix them, y or n, then works perfect, same as 4 base
4_base.txt		Base case, this is what my main testing was done on
4_neg_cycle.txt		Edited base case, works (in that it fails)
8_cw_loop.txt		These are kind of cool
8_ccw_loop.txt		These are kind of cool
50_random.txt		Negative cycle, it was a disaster
50_all_pos_random.txt	No neg cycle, this is impossible to test? lol
