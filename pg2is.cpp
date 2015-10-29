/*
Author: Andrew Winslow (andrewwinslow@gmail.com)

Program for converting a symbol-pair grammar to an insertion system
that expresses it, following the algorithm described in the proof 
of Lemma 3.3 of http://arxiv.org/abs/1401.0359

The program takes a symbol-pair grammar from stdin
and prints the resulting insertion system to stdout.
The output symbol-pair grammar can be used as input to simulator.

The input grammar must consist of lines (delimited by '\n'),
each with one of three forms:
1. "# ..." (a comment) or " " (whitespace).
2. "(1, 2) -> (1, 3) (4, 1)" (an integer 2-tuple followed by "->" 
                              followed by two integer 2-tuples whose 
                              first and last letters match those of
                              the first 2-tuple.)
3. "(1, 2) -> a" (an integer 2-tuple number followed by "->" 
                  followed by a letter.) 
The order of the lines does not matter.
*/

#include "pairgrammar.h"
#include <stdlib.h>
#include <stdio.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

void clean(char buf[]) {
	int i;
	for(i = 0; i < BUFSIZ && buf[i] != '\0'; i++) {
		// Comments
		if(buf[i] == '#') 
			buf[i] = '\0';
		// Right arrows
		else if(buf[i] == '-' || buf[i] == '>')
			buf[i] = ' ';
		// Tuple cruft 
 		else if (buf[i] == ',' || buf[i] == '(' || buf[i] == ')')
			buf[i] = ' ';
	}	
}

int main() {
	PairGrammar pg;

	char buf[BUFSIZ];
	PairGrammar::Rule r;
	while (fgets(buf, sizeof buf, stdin)) {
		clean(buf);		

		// Try to read the line as a (a, d) -> (a, b) (c, d) rule
		if (sscanf(buf,"%d %d %d %d %d %d", &r.lhs.a, &r.lhs.d, &r.rhs1.a, &r.rhs1.d, &r.rhs2.a, &r.rhs2.d) == 6) {
			r.is_terminal = false;	
			if (!PairGrammar::is_valid(r)) {
				fprintf(stderr, "Line was parsed as (%d, %d) -> (%d, %d) (%d, %d), an invalid rule:\n	%s\n",
					r.lhs.a, r.lhs.d, r.rhs1.a, r.rhs1.d, r.rhs2.a, r.rhs2.d, buf); 
				return EXIT_FAILURE;
			}
			pg.add_rule(r);
			continue;
		}

		// Try to read the line as a (a, d) -> s rule
		if (sscanf(buf,"%d %d %c", &r.lhs.a, &r.lhs.d, &r.rhsTerm) == 3) {
			r.is_terminal = true;
			if (!PairGrammar::is_valid(r)) {
				fprintf(stderr, "Line was parsed as (%d, %d) -> %c, an invalid rule:\n	%s\n",
					r.lhs.a, r.lhs.d, r.rhsTerm, buf);
				return EXIT_FAILURE;
			}
			pg.add_rule(r);
			continue;
		}	

		// Try to read the line as a start symbol (a, d)
		if (sscanf(buf, "%d %d", &r.lhs.a, &r.lhs.d) == 2) {
			if (pg.has_start()) {
				fprintf(stderr, "Line was parsed as (%d, %d), but pair grammar already has a start symbol:\n	%s\n",
					r.lhs.a, r.lhs.d, buf);
				return EXIT_FAILURE;
			}
			pg.set_start(r.lhs);
			continue;
		}

		if (sscanf(buf, "%d", &r.lhs.a) != EOF) {
			fprintf(stderr, "Line has stuff but can't be parsed:\n	%s\n", buf);
			return EXIT_FAILURE;
		}	

	}

	// Check that the resulting grammar is valid globally (something we can't do line by line)
	if (!pg.is_valid()) {
		fprintf(stderr, "Pair grammar is not valid.\n");
		return EXIT_FAILURE;
	}

	pg.print_insertion_system();
	return EXIT_SUCCESS;
}




