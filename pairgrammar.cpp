
#include "pairgrammar.h"

PairGrammar :: PairGrammar() {
	_has_start = false;
}

bool PairGrammar :: has_start() {
	return _has_start;
}

void PairGrammar :: set_start(Nonterminal nt) {
	if (_has_start)
		return;
	_start = nt;
	_has_start = true;
}

void PairGrammar :: add_rule(Rule r) {
	rules.push_back(r);
}

bool PairGrammar :: is_valid(Rule r) {
	if (r.is_terminal)
		return true;
	return (r.lhs.a == r.rhs1.a && r.lhs.d == r.rhs2.d);
}

// Checks whether the grammar is "valid", meaning that:
// 1. The grammar has a start symbol.
// 2. Every non-terminal in the grammar appears on the 
// left-hand side (lhs) of some rule.
bool PairGrammar :: is_valid() {
	if (!_has_start)
		return false;

	for (unsigned int i = 0; i < rules.size(); ++i) {
		Rule r = rules[i];
		// Check that rule is ok
		if (!is_valid(r)) 
			return false;

		// Check that each rhs symbol appears on lhs of some rule 
		if (r.is_terminal)
			continue;

		bool rhs1_found = false;
		bool rhs2_found = false;
		for (unsigned int j = 0; j < rules.size(); ++j) {
			rhs1_found = rhs1_found || (rules[j].lhs.a == r.rhs1.a && rules[j].lhs.d == r.rhs1.d); 
			rhs2_found = rhs2_found || (rules[j].lhs.a == r.rhs2.a && rules[j].lhs.d == r.rhs2.d); 
		}
	
		if (!rhs1_found || !rhs2_found)
			return false;	
	}

	return true;
}

void PairGrammar :: print() {
	printf("# Start symbol\n");
	printf("(%d, %d)\n", _start.a, _start.d);

	printf("# Rules\n");
	for (unsigned int i = 0; i < rules.size(); ++i) {
		Rule r = rules[i];
		if (r.is_terminal)	
			printf("(%d, %d) -> %c\n", r.lhs.a, r.lhs.d, r.rhsTerm);
		else
			printf("(%d, %d) -> (%d, %d) (%d, %d)\n", r.lhs.a, r.lhs.d, r.rhs1.a, r.rhs1.d, r.rhs2.a, r.rhs2.d);
	}	
}

void PairGrammar :: print_insertion_system() {
	// Compute range of non-terminal indices to enable finding values 
	// for u, x, and range for terminal characters that doesn't interfere
	// Compute min and max of the indices in all non-terminals
	vector<int> indices;
	for (unsigned int i = 0; i < rules.size(); ++i) {
		indices.push_back(rules[i].lhs.a);
		indices.push_back(rules[i].lhs.d);
		if (!rules[i].is_terminal) {
			indices.push_back(rules[i].rhs1.a);
			indices.push_back(rules[i].rhs1.d);
			indices.push_back(rules[i].rhs2.a);
			indices.push_back(rules[i].rhs2.d);
		}
	}
	int max_pg_index = *max_element(begin(indices), end(indices));	
	int u_index = max_pg_index + 1;
	int x_index = u_index + 1;
	int terminal_shift = x_index + 1;

	// Actually do the printing, following the proof of Lemma 3.3
	printf("# Insertion system generated by pg2is using algorithm from\n");
	printf("# B. Hescott, C. Malchik, A. Winslow,\n");
	printf("# \"Tight bounds for active self-assembly using an insertion primitive\",\n");
	printf("# http://arxiv.org/abs/1401.0359\n\n");

	printf("# Non-terminal integer indices range up to %d.\n", max_pg_index);
	printf("# u = %d, x = %d\n", u_index, x_index);
	printf("# Terminal characters range from %d to %d.\n\n", terminal_shift, terminal_shift + 256);

	printf("# Initiator for start symbol (%d, %d)\n", _start.a, _start.d);
	printf("(%d, %d) (%d, %d*)\n\n", u_index, _start.a, _start.d, u_index); 

	for (unsigned int i = 0; i < rules.size(); ++i) { 
		Rule r = rules[i];
		if (!r.is_terminal) {
			printf("# Monomers for rule (%d, %d) -> (%d, %d) (%d, %d).\n", 
				r.lhs.a, r.lhs.d, r.rhs1.a, r.rhs1.d, r.rhs2.a, r.rhs2.d);
			// Delta_1'
			printf("(%d, %d*, %d*, %d)-\n", r.rhs1.d, u_index, r.rhs1.d, x_index);      
			// Delta_2'
			printf("(%d*, %d, %d*, %d*)+\n", r.rhs1.a, r.rhs1.d, r.rhs2.a, r.rhs2.d); 
			// Delta_3'
			printf("(%d, %d, %d, %d)-\n", x_index, r.rhs2.a, u_index, r.rhs2.a); 
		} else {
			printf("# Monomers for rule (%d, %d) -> %c\n", r.lhs.a, r.lhs.d, r.rhsTerm);
			// Delta_4'
			printf("(%d*, %d, %d, %d*)+\n", r.lhs.a, r.rhsTerm + terminal_shift, x_index, r.lhs.d); 
		}
		printf("\n");
	}


}


