
#include "grammar.h"

// Checks whether the grammar is "valid", meaning that:
// 1. The grammar has a start symbol.
// 2. Every non-terminal in the grammar appears on the 
// left-hand side (lhs) of some rule.
bool grammar_is_valid(Grammar* g)
{
	if (!g->has_start)
		return false;

	bool rhs1_found, rhs2_found;
	for (unsigned int i = 0; i < g->rule_count; ++i) {
		int rhs1 = g->rules[i].rhs1;
		int rhs2 = g->rules[i].rhs2;
	
		rhs1_found = rhs2_found = false;
		for (unsigned int j = 0; j < g->rule_count; ++j) {
			rhs1_found = rhs1_found || (g->rules[j].lhs == rhs1); 
			rhs2_found = rhs2_found || (g->rules[j].lhs == rhs2); 
		}
	
		if (!rhs1_found || !rhs2_found)
			return false;	
	}

	return true;
}

void initialize_grammar(Grammar* g)
{
	g->has_start = false;
	g->rule_count = 0;
}

void add_rule_to_grammar(Rule r, Grammar* g)
{
	if (g->rule_count >= MAX_GRAMMAR_RULES)
		return;

	g->rules[g->rule_count] = r;
	++g->rule_count;
}

void set_start(int n, Grammar* g)
{
	if (g->has_start)
		return;

	g->start = n;
	g->has_start = true;
}


