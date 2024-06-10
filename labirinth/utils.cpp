#include <fstream> 
#include <iostream> 
#include <string> 

using namespace std; 

void read_map(string filename) {

	ifstream file(filename); 
	string line; 

	if (file.is_open()) { 
		// Read each line from the file and store it in the 
		// 'line' variable. 
		while (getline(file, line)) { 
			cout << "\t" << line << endl; 
		} 
  
		// Close the file stream once all lines have been 
		// read. 
		file.close(); 
	} 
	else { 
		// Print an error message to the standard error 
		// stream if the file cannot be opened. 
		cerr << "Unable to open file!" << endl; 
	} 

}

void test_read_map01() {

	cout << "Test read_map" << endl;
	read_map("map_test.map");

}