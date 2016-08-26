/*
Patrick Vielhaber - PTV1
12_1_2014 
Final Project
 */

using namespace std;

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

//Made a class to hold the results
//In this way it's possible to pass back the INTEGER array of shortest paths
//As well all the STRING array of the directions/parents of how each path was created
class result_holder
{
	public:
	
	string *directions;  //String array for directions
	int *shortest_pairs;  //Integer array for shortest pairs

	result_holder(); //Empty constructor
	//Constructor that makes the two arrays of correct size
	result_holder(int size){
		directions = new string[size * size];	
		shortest_pairs = new int[size * size];	
	};
	//Destructor that isn't used?  I had problems with this
	~result_holder(){
		//I feel like this should be here, but kept giving me segementation faults,
		//Do you know anything better about destructors?
		//cout << "Deconstructor" << endl;
		//delete [] directions;
		//delete [] shortest_pairs;
	}

};

//Weights should not be bigger than this
const int INFINITY = 99999;
//If there is no path, print this
const string NO_PATH = "N/A";



//This just prints the board, never really need this, nor used, except for debugging
void print_board(int *arr, int x, int y);
//This will print strings
void print_strings(string *arr, int x, int y);
//This will print the paths for all of them, which also isn't really needed
void print_paths(string *arr, int x, int y);
//This will return a results case from Floyds (shortest pairs, and the directions/paths)
result_holder floyds_algorithm(int *arr, int x, int y);
//We might not need this, but I wanted it implemented now regardless
bool find_neg_cycle(int *arr, int x, int y);
//Pre-check the matrix diagonals
bool check_diagonal_zeros(int *arr, int x, int y);
//This will separate the path into individual values, dunno if needed
int * deparser(string path, int max_size);


int main(int argc, char *argv[]) {

    if (argc != 2 && argc != 4) {
        cout << "Wrong input format!" << endl << endl;
        cout << "Please run the program with one text file argument." << endl
                << "The text file should be a maxtrix where the first line is one length of the square array." << endl
                << "A 10 by 10 matrix's first line should be 10, not 100." << endl
                << "Example 3 x 3 matrix" << endl
                << "    A     B     C" << endl
                << "A     0     2     3" << endl
                << "B     1     0 99999" << endl
                << "C 99999     5     0" << endl << endl
                << "A conntects to B (weight 2), C (weight 3)" << endl
                << "B conntects to A (weight 1)" << endl
                << "C conntects to B (weight 5)" << endl
                << "C DOES NOT connect to A (99999 = infinity)" << endl
                << "B DOES NOT connect to C (99999 = infinity)" << endl
				<< "The weight for traveling from node 'i' to node 'i' should always be zero." << endl
				<< "(All the values along the diagonal should be equal to zero.)" << endl
                << "Such as --main 3x3_graph.txt--" << endl
				<< "--You can also submit the matrix text file, and a starting and ending point" << endl
				<< "To get the shortest path for that specific combo as well." << endl 
				<< "Such as --main 3x3_graph.txt 0 2-- to traval from A to C in the previously mentioned matrix." << endl;

        exit(1);
    }  

    //Use this to turn different 'cout' statements on and off, this is nicer than commenting them out
    bool bugger;
    bugger = true;


    string full_filename(argv[1]);

    int size_x = 0;
    int size_y = 0;

    cout << "----READING IN FILE----> " << full_filename << endl;

    //Read in the file
    ifstream read_file;
    read_file.open(full_filename.c_str());
    read_file >> size_x;
    size_y = size_x;

    cout << "Graphs is size: " << size_x << " wide by " << size_y << " tall." << endl << endl;;

    int *graph_matrix = new int[size_x * size_y];
    int i = 0;
    while (!read_file.eof()) {
        int x;
        read_file >> x;
        graph_matrix[i] = x;
        i++;
    }

	//This would just print their original graph, we probably don't need this here? Comment out?
	cout << "Original Graph Matrix";
    print_board(graph_matrix, size_x, size_y);

	if (check_diagonal_zeros(graph_matrix, size_x, size_y)){
		cout << "The matrix file given did not have zero weights for self node travel." << endl;
		cout << "Would you like me to fix them (set the weights along the matrix's diagonal to zero) and continue?" << endl;
		string temp;
		getline(cin, temp);
		if (temp[0] == 'n' || temp[0] == 'N'){
			cout << "Program End" << endl;
			return 0;
		}
	}

	//This would reprint if you wanted to check the diagonals - Def don't need this IMO
	//print_board(graph_matrix, size_x, size_y);

	//Make a class to hold the results, this way we can pass shortest path matrix, and the directions matrix in/out of Floyds
    result_holder final_solution(size_x);

	//This will run Floyds and pass back the pairs and paths
    final_solution = floyds_algorithm(graph_matrix, size_x, size_y);





	//And then change the outputs and stuff based on if they gave us starting / ending points
	if (argc == 2){
		cout << "You have not given any starting or ending points." << endl
			<< "This method of running the program is more for checking and debugging." << endl
			<< "For larger graphs, this may not yield results that are formatted very well." << endl
			<< "But nonetheless, here is a bunch of information for your given graph." << endl << endl;

		//Print the shortest pairs matrix
		cout << "Shortest Pairs Matrix";
    	print_board(final_solution.shortest_pairs, size_x, size_y);

		//Print all the directions, why not
		cout << "This is all the paths, this will get messy as some paths are longer";
		print_strings(final_solution.directions, size_x, size_y);

		//Print every single one of the path directions - This is the super long one
		print_paths(final_solution.directions, size_x, size_y);

		cout << "-----Negative Cycle Check-----" << endl;
		cout << "The given matrix ";
		if (find_neg_cycle(final_solution.shortest_pairs, size_x, size_y))
			 cout << "DOES ";
		else cout << "does NOT ";
		cout << "have a negative cycle." << endl << endl;

	} else if (argc == 4){

		//Make an array to hold the de-parsed path integers
		int * shortest_nodes = new int[size_x];
		//This is just random test values in here
		//There would be a check before this ever got here to make sure start/end we within the graphs range of nodes
		int starting_point = atof(argv[2]);
		int ending_point = atof(argv[3]);
		
		if (starting_point >= size_x || ending_point >= size_x ||
			starting_point < 0 || ending_point < 0) {
			cout << "Your starting or ending point does not fall in the given graph." << endl
				<< "Note: A 4x4 graph only has nodes 0, 1, 2, and 3." << endl;
		
		} else {
			cout << "Here is the path to travel" << endl 
				<< "From the given starting node: " << starting_point << endl
				<< "    To the given ending node: " << ending_point << endl << endl;
		

			shortest_nodes = deparser(final_solution.directions[starting_point * size_x + ending_point], size_x);





			//The de-parser will send back a -1 if there is no path
			cout << "-----Printing The Specific Path-----" << endl;
			if (shortest_nodes[0] != -1) {
				for (int i = 0; i < size_x; i++){
					//This means there is some other piece of data allocated in the array, so we can peek it
					if (i < size_x - 1) { 
						//There is another value to print, so print the two dashes too
						if (shortest_nodes[i + 1] != -1)
							cout << shortest_nodes[i] << " --> ";
						else {
							//If the next one is a kill value, don't print anymore
							cout << shortest_nodes[i];
							i = size_x;   //Kill the for loop
						}
					} else cout << shortest_nodes[i];
				}
				cout << endl;
				
				cout << "The weight for this path is " << final_solution.shortest_pairs[starting_point * size_x + ending_point] << endl;
			} else {
				cout << "There is no path that connects node " << starting_point << " to node " << ending_point << "." << endl;
			}
			
			

		delete [] shortest_nodes;
		}
	} 

	//Clean up and end 
	delete [] graph_matrix;
	cout << endl;
    return 0;
}
//END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM
//END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM
//END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM
//END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM -- END OF PROGRAM
//cout << "--------We got to here--------" << endl << endl;






//This takes a string direction that are separated by dashes, and deparses them, only does one string at a time
int * deparser(string path, int max_size) {
	int * temp = new int[max_size];
	int start = 0;
	int end = 0;
	
	//Bunch of commented out debugging cout's here
	//cout << "-------------De-parsing Beginning-------------" << endl;
	
	//This is super useful
	//cout << "This is the string: " << path << " and it's " << path.length() << " chars long. " << endl;
	
	//The first start will always be zero
	//Find the first end
	end = path.find("-");
	
	//If we didn't find any dashes, send back neg value in the array
	if (end == -1) {
		temp[0] = -1;
	} else {
		for (int i = 0; i < max_size; i++) {
			string temp_parser = "";
			temp_parser = path.substr(start, end);
			istringstream convert_x(temp_parser);
			convert_x >> temp[i] ;

			//cout << "Start/End/Value: " << start << "/" << end << "/" << temp[i] << endl;

			//Move the starting point to the ending point plus one
			start = end + 1;
			end = path.find("-",start);
			//cout << "End each time == " << end << endl;
			//cout << "Start each time == " << start << endl;			
			if (end == -1) end = path.length() + 1;
			
			//If start ever gets bigger than the full length you know we've de-parsed the whole string
			if (start > path.length()) {
				//If the array didn't get filled, give MAIN a kill sentinal value
				if (i < max_size - 1)
					temp[i+1] = -1;
				i = max_size;				
			}

		}
	}
	return temp;
}

//This is Floyd's Algorithm for shortest path
result_holder floyds_algorithm(int *arr, int x, int y) {
    //Make an instance of the class to hold the results
	result_holder results(x);
	
	//I wanted this for debugging when and edge got changed
	//We can remove / comment out later, but this will help with 'directions'
	string *edge_got_edited = new string[x * y];
	
    int index = 0;

    //Just copy over the other matrix, to keep original
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            index = (i * x) + j;
            results.shortest_pairs[index] = arr[index]; //Copy them over
			edge_got_edited[index] = " ";  //Space them to start
			results.directions[index] = "";  //Blank them to start
        }
    }

	//Prep a converter variable for string/int for the directions
	stringstream converter;
	converter.str("");
	
	int horizontal_index = 0;
	int vertical_index = 0;
	
    //Now to the Floyd's Algorithm's runs to find all the shortest pairs
    for (int runs = 0; runs < x; runs++) {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                index = i * x + j;
				horizontal_index = i * x + runs;
				vertical_index = runs * x + j;
				//We can only do this if the two connecting cells aren't infinity
				//We handled "INFINITY" by making a globabl variable and "really big"
				if (results.shortest_pairs[horizontal_index] < INFINITY && results.shortest_pairs[vertical_index] < INFINITY){
                	//cout << "here" << i << "--" << j << endl;
					if (results.shortest_pairs[horizontal_index] + results.shortest_pairs[vertical_index] < results.shortest_pairs[index]) {
                    	results.shortest_pairs[index] = results.shortest_pairs[horizontal_index] + results.shortest_pairs[vertical_index];
						edge_got_edited[index] = "*";  //This will let me know it changed
						//Clear the previous instructions, since we've found a new shortest path
						results.directions[index] = ""; 
						converter << runs;
											
						//We have to look at the edges' directions that were used to get here
						//Put in any previous paths it took to get to the current RUN node first
						if (results.directions[horizontal_index] != "") {
							//if (i == 2 && j == 0) cout << converter.str() << endl;
							results.directions[index] = results.directions[horizontal_index];
						}
						
						//if (i == 2 && j == 0) cout << results.directions[index] << endl;
						
						//Then you append the current run node, you'll always use this
						results.directions[index] = results.directions[index] + converter.str() + "-";
						
						//Put in any previous paths it took to get to the current RUN node first
						if (results.directions[vertical_index] != "")
							results.directions[index] = results.directions[index] + results.directions[vertical_index];

						converter.str("");
					}
                }
            }
        }
		
		//THIS IS JUST FOR DEBUGGING!!
		bool small_bugger = false;
		//This is the internal printer.  Just turn this variable = false to disable, 
		//It will be a mess for big graphs
		if (small_bugger) {
    		cout << endl << "------Board Print------" << endl;
    		int index = 0;
    		for (int i = 0; i < y; i++) {
        		for (int j = 0; j < x; j++) {
            		index = (i * x) + j;
					
            		cout << setw(7) << results.shortest_pairs[index];
					cout << edge_got_edited[index];
					edge_got_edited[index] = " "; //Reset it
        		}
        		cout << endl;
    		}
    	}
	}
		
	//Format the paths so they make sense (Cap them with their respective starting/ending nodes)
	stringstream conv1;
	stringstream conv2;
	conv1.str("");
	conv2.str("");	
	
	//Put the end caps on all the paths, just cuz
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            index = (i * x) + j;
			conv1 << i;
			conv2 << j;
			
			//If the shortest path is still infinity, we have issues
			if (results.shortest_pairs[index] == INFINITY)
				results.directions[index] = NO_PATH;
			//If no intermediate paths were utilized, the shortest parth was simply from i to j
			else if (results.directions[index] == "")
				results.directions[index] = conv1.str() + "-" + conv2.str();
			else {
				results.directions[index] = conv1.str() + "-" + results.directions[index] + conv2.str();
			}
			conv1.str("");
			conv2.str("");
        }
    }	
	
    return results;
}

//This will be a pre-check to make sure that all the 'i,i' nodes are zeros
bool check_diagonal_zeros(int *arr, int x, int y){
	bool bad_matrix = false; //Assume it's ok
    for (int i = 0; i < x; i++) {
		//If any diagonals are not zero, we know there's something wrong with the matrix
	    if (arr[i * x + i] != 0) {
			arr[i * x + i] = 0;  //Reset them just if they want us to
			bad_matrix =  true;  //But don't quit just yet
        }
    }
	return bad_matrix;
}

//Only run this on a shortest pairs matrix
//This will check the diagonals (traveling to from node 'i' to node 'i'
bool find_neg_cycle(int *arr, int x, int y) {
    for (int i = 0; i < x; i++) {
		//If any diagonals are zero, we know there's a negative cycle
	    if (arr[i * x + i] < 0) {
			return true;
        }
    }
	return false;
}

//This will print simple boards, big board might be weird
void print_board(int *arr, int x, int y) {
    cout << endl << "------Int Board Print------" << endl;
    int index = 0;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            index = (i * x) + j;
			cout << setw(7);
			if (arr[index] == INFINITY)
				cout << "INF";
            else cout << arr[index];
        }
        cout << endl;
    }
	cout << endl;
}

//This will print strings
void print_strings(string *arr, int x, int y) {
    cout << endl << "------String Board Print------" << endl;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
			cout << setw(2 * (x + 1)) << arr[i * x + j];
        }
        cout << endl;
    }
	cout << endl;
}

//This will print the paths, not super useful
void print_paths(string *arr, int x, int y){
	cout << endl << "------Path Print------" << endl;
	int index = 0;
	for (int i = 0; i < y; i++){
		for (int j = 0; j < x; j++){
			if (i != j){
				index = i * x + j;
				if (arr[index] == NO_PATH)
					cout << "There is no path from " << i << " to " << j << "." << endl;
				else cout << "The shortest path from " << i << " to " << j << " is: " << arr[index] << endl;
			}
		}
	}
	cout << endl;
}















