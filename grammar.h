
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "pairgrammar.h"
#include <vector>

using std::vector;

class Grammar {

	public:
		typedef struct {
			bool is_terminal;
			int lhs;
			int rhs1, rhs2;
			char rhsTerm;
		} Rule;

		Grammar();
		int start();
		bool has_start();	
		bool is_valid();
		void set_start(int n);
		void add_rule(Rule r);
		void normalize();
		void print_grammar();
		PairGrammar pairgrammar();	
	
	private:
		int _start;	
		bool _has_start;
		vector<Rule> rules;
};

#endif

