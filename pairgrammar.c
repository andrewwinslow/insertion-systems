
#include "pairgrammar.h"

// Checks whether the rule is "valid", meaning that:
// 1. If the rule is (a, b) -> (c, d) (e, f), a = c and b = f
bool rule_is_valid(Rule* r)
{
	if (r->is_terminal)
		return true;

	return ((r->lhs.a == r->rhs1.a) && (r->lhs.d == r->rhs2.d));
}

// Checks whether the grammar is "valid", meaning that:
// 1. The grammar has a start symbol.
// 2. Every non-terminal in the grammar appears on the 
// left-hand side (lhs) of some rule.
bool pair_grammar_is_valid(PairGrammar* pg)
{
	if (!pg->has_start)
		return false;

	bool rhs1_found, rhs2_found;
	for (unsigned int i = 0; i < pg->rule_count; ++i) {
		if (!rule_is_valid(&pg->rules[i]))
			return false;

		Nonterminal rhs1 = pg->rules[i].rhs1;
		Nonterminal rhs2 = pg->rules[i].rhs2;
	
		rhs1_found = rhs2_found = false;
		for (unsigned int j = 0; j < pg->rule_count; ++j) {
			rhs1_found = rhs1_found || (pg->rules[j].lhs.a == rhs1.a && pg->rules[j].lhs.d == rhs1.d); 
			rhs2_found = rhs2_found || (pg->rules[j].lhs.a == rhs2.a && pg->rules[j].lhs.d == rhs2.d); 
		}
	
		if (!rhs1_found || !rhs2_found)
			return false;	
	}

	return true;
}

void initialize_pair_grammar(PairGrammar* pg)
{
	pg->has_start = false;
	pg->rule_count = 0;
}

void add_rule_to_pair_grammar(Rule r, PairGrammar* pg)
{
	if (pg->rule_count >= MAX_PAIR_GRAMMAR_RULES)
		return;

	pg->rules[pg->rule_count] = r;
	++pg->rule_count;
}

void set_start(Nonterminal n, PairGrammar* pg)
{
	if (pg->has_start)
		return;

	pg->start = n;
	pg->has_start = true;
}


