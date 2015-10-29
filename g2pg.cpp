/*
Author: Andrew Winslow (andrewwinslow@gmail.com)

Program for converting a grammar to a symbol-pair grammar
following the algorithm described in the proof of Lemma 3.2 of
http://arxiv.org/abs/1401.0359

The program takes a (Chomsky normal form) grammar from stdin 
and prints the resulting symbol-pair grammar to stdout. 
The output symbol-pair grammar can be used as input to pg2is.

The input grammar must consist of lines (delimited by '\n'),
each with one of three forms:
1. "# ..." (a comment) or " " (whitespace).
2. "1 -> 2 3" (a number followed by "->" followed by two more numbers)
3. "1 -> a" (a number followed by "->" followed by a letter) 
The order of the lines does not matter.
*/

#include "grammar.h"
#include <string>
#include <iostream>

using std::string;

// Removes comment lines and right arrows parts ('-', '>')
void clean_line(string &l) {
	for (unsigned int i = 0; i < l.size(); ++i) {
		// Comments
		if (l[i] == '#')
			l.erase(i);
		else if (l[i] == '-' || l[i] == '>')
			l[i] = ' ';
	}	
}

int main() {
	Grammar g;

	Grammar::Rule tempRule;
	string line;
	while (std::cin) {
		getline(std::cin, line); 
		clean_line(line);

		if (sscanf(line.c_str(), "%d %d %d", &tempRule.lhs, &tempRule.rhs1, &tempRule.rhs2) == 3) {
			tempRule.is_terminal = false;	
			g.add_rule(tempRule); 
			continue;
		}

		// Try to read the line as a (a, d) -> s rule
		if (sscanf(line.c_str(), "%d %c", &tempRule.lhs, &tempRule.rhsTerm) == 2) {
			tempRule.is_terminal = true;
			g.add_rule(tempRule); 
			continue;
		}	

		// Try to read the line as a start symbol (a, d)
		if (sscanf(line.c_str(), "%d", &tempRule.lhs) == 1) {
			if (g.has_start()) {
				std::cerr << "Line was parsed as " << tempRule.lhs << ", but grammar already has start symbol." << std::endl;
				return EXIT_FAILURE;
			}
			g.set_start(tempRule.lhs);
			continue;
		}

		// Try to read the line as the end of the file
		if (line.find_first_not_of("\n\t") != std::string::npos) {
			std::cerr << "Line has stuff but can't be parsed:" << std::endl << line << std::endl;
			return EXIT_FAILURE;
		}	
	}

	// Check that the resulting grammar is valid 
	if (!g.is_valid()) {
		fprintf(stderr, "Grammar is not valid.\n");
		return EXIT_FAILURE;
	}
	
	// Compute and output the pair grammar
	g.pairgrammar().print();

	return EXIT_SUCCESS;
}




