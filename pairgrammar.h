
#ifndef PAIRGRAMMAR_H
#define PAIRGRAMMAR_H

#include <vector>

using std::vector;

class PairGrammar {

	public:
		typedef struct {
			int a, d;
		} Nonterminal;

		typedef struct {
			bool is_terminal;
			Nonterminal lhs;
			Nonterminal rhs1, rhs2;
			char rhsTerm;
		} Rule;

		PairGrammar();
		int start();
		bool has_start();
		bool is_valid();
		void set_start(Nonterminal nt);
		void add_rule(Rule r);
		void print();
		void print_insertion_system();
		
		static bool is_valid(Rule r);

	private:
		Nonterminal _start;
		bool _has_start;
		vector<Rule> rules;
};

#endif


