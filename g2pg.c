/*
Copyright (c) 2015, Andrew Winslow (andrewwinslow@gmail.com)

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*
Program for converting a grammar to a symbol-pair grammar
following the algorithm described in the proof of Lemma 3.2 of
http://arxiv.org/abs/1401.0359

The program takes a (Chomsky normal form) grammar from stdin 
and prints the resulting symbol-pair grammar to stdout. 
The output symbol-pair grammar can be used as input to pg2is.

The input grammar must consist of lines (delimited by '\n'),
each with one of three forms:
1. "# ..." (a comment)
2. "1 -> 2 3" (a number followed by "->" followed by two more numbers)
3. "1 -> a" (a number followed by "->" followed by a letter) 
The order of the lines does not matter.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grammar.h"

int min(int a, int b) {
	return (a < b ? a : b);
}

int max(int a, int b) {
	return (a > b ? a : b);
}

// Removes comment lines and right arrows parts ('-', '>')
void clean(char buf[]) 
{
	int i;
	for(i = 0; i < BUFSIZ && buf[i] != '\0'; i++) {
		// Comments
		if(buf[i] == '#') 
			buf[i] = '\0';
		// Right arrows
		else if(buf[i] == '-' || buf[i] == '>')
			buf[i] = ' ';
	}	
}

int main()
{
	Grammar* g = malloc(sizeof(Grammar));
	initialize_grammar(g);

	char buf[BUFSIZ];
	Rule tempRule;
	while (fgets(buf, sizeof buf, stdin)) {
		clean(buf);		

		// Try to read the line as a (a, d) -> (a, b) (c, d) rule
		if (sscanf(buf,"%d %d %d", &tempRule.lhs, &tempRule.rhs1, &tempRule.rhs2) == 3) {
			tempRule.is_terminal = false;	
			add_rule_to_grammar(tempRule, g); 
			continue;
		}

		// Try to read the line as a (a, d) -> s rule
		if (sscanf(buf,"%d %c", &tempRule.lhs, &tempRule.rhsTerm) == 2) {
			tempRule.is_terminal = true;
			add_rule_to_grammar(tempRule, g); 
			continue;
		}	

		// Try to read the line as a start symbol (a, d)
		if (sscanf(buf, "%d", &tempRule.lhs) == 1) {
			if (g->has_start) {
				fprintf(stderr, "Line was parsed as %d, but grammar already has a start symbol:\n	%s\n",
					tempRule.lhs, buf);
				return EXIT_FAILURE;
			}
			set_start(tempRule.lhs, g);
			continue;
		}

		if (sscanf(buf, "%d", &tempRule.lhs) != EOF)
		{
			fprintf(stderr, "Line has stuff but can't be parsed:\n	%s\n", buf);
			return EXIT_FAILURE;
		}	

	}

	// Check that the resulting grammar is valid globally (something we can't do line by line)
	if (!grammar_is_valid(g))
	{
		fprintf(stderr, "Grammar is not valid.\n");
		return EXIT_FAILURE;
	}

	// Compute min and max of the indices in all non-terminals
	int min_g_index = 0;
	int max_g_index = 0;
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		Rule* r = &g->rules[i];
		if (r->is_terminal) {
			min_g_index = min(min_g_index, r->lhs);
			max_g_index = max(max_g_index, r->lhs);
		}
		else {
			min_g_index = min(min(min(min_g_index, r->lhs), r->rhs1), r->rhs2);
			max_g_index = max(max(max(max_g_index, r->lhs), r->rhs1), r->rhs2);
		}
	}
	// Shift the non-terminal indices to have minimum at 0 
	g->start += -min_g_index;
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		Rule* r = &g->rules[i];
		r->lhs += -min_g_index;	
		r->rhs1 += -min_g_index;	
		r->rhs2 += -min_g_index;	
	}	
	max_g_index += -min_g_index;

	// Shift the non-terminal indices to be 0 through n-1, with 0 being the start symbol
	// First, swap start with 0
	int old_start = g->start;
	g->start = 0;
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		Rule* r = &g->rules[i];
		if (r->lhs == 0)
			r->lhs = -1;
		if (r->rhs1 == 0)
			r->rhs1 = -1;
		if (r->rhs2 == 0)
			r->rhs2 = -1;
	}
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		Rule* r = &g->rules[i];
		if (r->lhs == old_start)
			r->lhs = 0;
		if (r->rhs1 == old_start)
			r->rhs1 = 0;
		if (r->rhs2 == old_start)
			r->rhs2 = 0;
	}
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		Rule* r = &g->rules[i];
		if (r->lhs == -1)
			r->lhs = old_start;
		if (r->rhs1 == -1)
			r->rhs1 = old_start;
		if (r->rhs2 == -1)
			r->rhs2 = old_start;
	}
	// Second, find all distinct non-terminals and count how many there are
	bool* present = malloc(sizeof(bool)*(max_g_index+1));
	for (unsigned int i = 0; i < max_g_index+1; ++i) {
		present[i] = false;
	}	
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		present[g->rules[i].lhs] = true;
		present[g->rules[i].rhs1] = true;
		present[g->rules[i].rhs2] = true;
	}
	int n = 0;
	for (unsigned int i = 0; i < max_g_index + 1; ++i) {
		if (present[i])
			++n;
	}
	// Third, change non-terminal indices until they form a solid block from 0 up to n-1
	int low_index = 1; // we already have 0 covered by the start symbol
	for (unsigned int i = n; i < max_g_index + 1; ++i) {
		if (present[i]) {
			while (present[low_index])
				++low_index;
			// now change all occurrances of i in grammar with low index
			for (unsigned int j = 0; j < g->rule_count; ++j) {
				if (g->rules[j].lhs == i)
					g->rules[j].lhs = low_index;
				if (g->rules[j].rhs1 == i)
					g->rules[j].rhs1 = low_index;
				if (g->rules[j].rhs2 == i)
					g->rules[j].rhs2 = low_index;
			}
			// change present array to be correct
			present[i] = false;
			present[low_index] = true;
		}
	}
	free(present);	

	fprintf(stdout, "# Pair grammar generated by g2pg using algorithm from\n");
	fprintf(stdout, "# B. Hescott, C. Malchik, A. Winslow, \"Tight bounds for active self-assembly using an insertion primitive\"\n");
	fprintf(stdout, "http://arxiv.org/abs/1401.0359\n\n");
	fprintf(stdout, "# Start symbol\n");
	fprintf(stdout, "(0, 0)\n\n");	
	for (unsigned int i = 0; i < g->rule_count; ++i) { 
		Rule* r = &g->rules[i];
		if (r->is_terminal) {
			fprintf(stdout, "# Rules for the rule %d -> %c\n", r->lhs, r->rhsTerm);
			for (unsigned int a = 0; a < n; ++a) {
				int d = (r->lhs - a + n) % n;
				fprintf(stdout, "(%d, %d) -> %c\n", a, d, r->rhsTerm);
			}
		} else {	
			fprintf(stdout, "# Rules for the rule %d -> %d %d\n", r->lhs, r->rhs1, r->rhs2);
			for (unsigned int a = 0; a < n; ++a) {
				int d = (r->lhs - a + n) % n;
				int b = (r->rhs1 - a + n) % n;
				int c = (r->rhs2 - d + n) % n;
				fprintf(stdout, "(%d, %d) -> (%d, %d) (%d, %d)\n", a, d, a, b, c, d);
			}
		}
		fprintf(stdout, "\n");
	}

	return EXIT_SUCCESS;
}




