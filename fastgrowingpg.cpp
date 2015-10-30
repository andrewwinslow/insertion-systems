/*
Author: Andrew Winslow (andrewwinslow@gmail.com)

Program for generating a pair grammar that deterministically
derives a string whose length is exponential in the number of rules.

The program takes a positive integer r as a command-line argument,
and prints a pair grammar with Theta(r) rules that deterministically
derives a string of length 2^Theta(r) to stdout. 
This output can be piped directly into pg2is.
*/

#include <iostream>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Error: no parameter k > 0 for the size of the pair grammar provided." << endl;
		return EXIT_FAILURE;
	}

	int k = atoi(argv[1]);

	cout << "# Start symbol" << endl;
	cout << "(1, 1)" << endl << endl;

	for (unsigned int i = 1; i <= k; ++i) {
		cout << "# Step " << i << endl;
		cout << "(" << i << ", " << i << ") -> (" << i << ", " << i+1 << ") (" << i+1 << ", " << i << ")" << endl;
		cout << "(" << i << ", " << i+1 << ") -> (" << i << ", " << 0 << ") (" << i+1 << ", " << i+1 << ")" << endl;
		cout << "(" << i+1 << ", " << i << ") -> (" << i+1 << ", " << i+1 << ") (" << 0 << ", " << i << ")" << endl;
		cout << "(" << i << ", 0) -> a" << endl;
		cout << "(0, " << i << ") -> a" << endl << endl;	
	}
	cout << "# Finishing rule" << endl;
	cout << "(" << k+1 << ", " << k+1 << ") -> a" << endl;

	return EXIT_SUCCESS;
}

