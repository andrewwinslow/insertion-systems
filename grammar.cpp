
#include "grammar.h"
#include <algorithm>

using std::min;
using std::max;
using std::begin;
using std::end;
using std::vector;

Grammar :: Grammar() {
	_has_start = false;
}

bool Grammar :: has_start() {
	return _has_start;
}

void Grammar :: set_start(int n) {
	if (_has_start)
		return;
	_start = n;
	_has_start = true;
}

// Checks whether the grammar is valid, namely that:
// 1. The grammar has a start symbol.
// 2. Every non-terminal in the grammar appears on the 
// left-hand side (lhs) of some rule.
bool Grammar :: is_valid() {
	if (!_has_start)
		return false;

	bool rhs1_found, rhs2_found;
	for (unsigned int i = 0; i < rules.size(); ++i) {
		int rhs1 = rules[i].rhs1;
		int rhs2 = rules[i].rhs2;
	
		rhs1_found = rhs2_found = false;
		for (unsigned int j = 0; j < rules.size(); ++j) {
			rhs1_found = rhs1_found || (rules[j].lhs == rhs1); 
			rhs2_found = rhs2_found || (rules[j].lhs == rhs2); 
		}
	
		if (!rhs1_found || !rhs2_found)
			return false;	
	}

	return true;
}

void Grammar :: add_rule(Rule r) {
	rules.push_back(r);
}

// Puts the grammar symbols into a normalized form, where they are 
// 0, 1, ..., n-1 and 0 is the start symbol. 
void Grammar :: normalize() {
        // Compute min and max of the indices in all non-terminals
	vector<int> indices;	
        for (unsigned int i = 0; i < rules.size(); ++i) {
		indices.push_back(rules[i].lhs);
                if (!rules[i].is_terminal) {
			indices.push_back(rules[i].rhs1);
			indices.push_back(rules[i].rhs2);
                }
        }
	int min_g_index = *min_element(begin(indices), end(indices));
        int max_g_index = *max_element(begin(indices), end(indices));

        // Shift the non-terminal indices to have minimum at 0 
        _start += -min_g_index;
        for (unsigned int i = 0; i < rules.size(); ++i) {
                rules[i].lhs += -min_g_index;
                rules[i].rhs1 += -min_g_index;
                rules[i].rhs2 += -min_g_index;
        }
        max_g_index += -min_g_index;		

        // Shift the non-terminal indices to be 0 through n-1, with 0 being the start symbol
        // First, swap start with 0
        int old_start = _start;
        _start = 0;
        for (unsigned int i = 0; i < rules.size(); ++i) {
                if (rules[i].lhs == 0)
                        rules[i].lhs = -1;
                if (rules[i].rhs1 == 0)
                        rules[i].rhs1 = -1;
                if (rules[i].rhs2 == 0)
                        rules[i].rhs2 = -1;
        }
        for (unsigned int i = 0; i < rules.size(); ++i) {
                if (rules[i].lhs == old_start)
                        rules[i].lhs = 0;
                if (rules[i].rhs1 == old_start)
                        rules[i].rhs1 = 0;
                if (rules[i].rhs2 == old_start)
                        rules[i].rhs2 = 0;
        }
        for (unsigned int i = 0; i < rules.size(); ++i) {
                if (rules[i].lhs == -1)
                        rules[i].lhs = old_start;
                if (rules[i].rhs1 == -1)
                        rules[i].rhs1 = old_start;
                if (rules[i].rhs2 == -1)
                        rules[i].rhs2 = old_start;
        }
        // Second, find all distinct non-terminals and count how many there are
	vector<bool> present;
	for (unsigned int i = 0; i < max_g_index+1; ++i)
		present.push_back(false);
        for (unsigned int i = 0; i < rules.size(); ++i) {
                present[rules[i].lhs] = true;
                present[rules[i].rhs1] = true;
                present[rules[i].rhs2] = true;
        }
        int n = 0;
        for (unsigned int i = 0; i < max_g_index + 1; ++i)
                n += (present[i] ? 1 : 0);
        // Third, change non-terminal indices until they form a solid block from 0 up to n-1
        int low_index = 1; // we already have 0 covered by the start symbol
        for (unsigned int i = n; i < max_g_index + 1; ++i) {
                if (present[i]) {
                        while (present[low_index])
                                ++low_index;
                        // now change all occurrances of i in grammar with low index
                        for (unsigned int j = 0; j < rules.size(); ++j) {
                                if (rules[j].lhs == i)
                                        rules[j].lhs = low_index;
                                if (rules[j].rhs1 == i)
                                        rules[j].rhs1 = low_index;
                                if (rules[j].rhs2 == i)
                                        rules[j].rhs2 = low_index;
                        }
                        // change present array to be correct
                        present[i] = false;
                        present[low_index] = true;
                }
        }
}

void Grammar :: print_grammar() {
	if (!is_valid())
		return;

	fprintf(stdout, "# Start symbol\n%d\n\n# Rules\n", _start);
	for (unsigned int i = 0; i < rules.size(); ++i) {
                if (rules[i].is_terminal) 
                        fprintf(stdout, "%d -> %c\n", rules[i].lhs, rules[i].rhsTerm);
		else
                        fprintf(stdout, "%d -> %d %d\n", rules[i].lhs, rules[i].rhs1, rules[i].rhs2);
	}
}

// Print out the pair grammar 
void Grammar :: print_pairgrammar() {
        fprintf(stdout, "# Pair grammar generated by g2pg using algorithm from\n");
        fprintf(stdout, "# B. Hescott, C. Malchik, A. Winslow,\n");
	fprintf(stdout, "# \"Tight bounds for active self-assembly using an insertion primitive\",\n");
        fprintf(stdout, "# http://arxiv.org/abs/1401.0359\n\n");

        fprintf(stdout, "# Start symbol\n");
        fprintf(stdout, "(0, 0)\n\n");
        
	normalize(); // Relabel the grammar symbols to being 0, 1, ..., n-1 with 0 being start symbol
	int n = rules.size();

        for (unsigned int i = 0; i < rules.size(); ++i) {
                if (rules[i].is_terminal) {
                        fprintf(stdout, "# Rules for the rule %d -> %c\n", rules[i].lhs, rules[i].rhsTerm);
                        for (unsigned int a = 0; a < n; ++a) {
                                int d = (rules[i].lhs - a + n) % n;
                                fprintf(stdout, "(%d, %d) -> %c\n", a, d, rules[i].rhsTerm);
                        }
                } else {
                        fprintf(stdout, "# Rules for the rule %d -> %d %d\n", rules[i].lhs, rules[i].rhs1, rules[i].rhs2);
                        for (unsigned int a = 0; a < n; ++a) {
                                int d = (rules[i].lhs - a + n) % n;
                                int b = (rules[i].rhs1 - a + n) % n;
                                int c = (rules[i].rhs2 - d + n) % n;
                                fprintf(stdout, "(%d, %d) -> (%d, %d) (%d, %d)\n", a, d, a, b, c, d);
                        }
                }
                fprintf(stdout, "\n");
        }
}

