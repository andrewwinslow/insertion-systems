
#include <stdbool.h>

#define MAX_PAIR_GRAMMAR_RULES 1000

typedef struct {
	int a, d;
} Nonterminal;

typedef struct {
        bool is_terminal;
	Nonterminal lhs;
	Nonterminal rhs1, rhs2;
	char rhsTerm;
} Rule;

typedef struct {
        bool has_start;
       	Nonterminal start; 
        Rule rules[MAX_PAIR_GRAMMAR_RULES];
        unsigned int rule_count;
} PairGrammar;

bool rule_is_valid(Rule* r);
bool pair_grammar_is_valid(PairGrammar* pg);
void initialize_pair_grammar(PairGrammar* pg);
void add_rule_to_pair_grammar(Rule r, PairGrammar* pg);
void set_start(Nonterminal n, PairGrammar* pg);

