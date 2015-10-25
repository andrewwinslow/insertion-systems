
#include <stdbool.h>

#define MAX_GRAMMAR_RULES 1000

typedef struct {
        bool is_terminal;
	int lhs;
	int rhs1, rhs2;
	char rhsTerm;
} Rule;

typedef struct {
        bool has_start;
       	int start;
        Rule rules[MAX_GRAMMAR_RULES];
        unsigned int rule_count;
} Grammar;


bool grammar_is_valid(Grammar* pg);
void initialize_grammar(Grammar* pg);
void add_rule_to_grammar(Rule r, Grammar* pg);
void set_start(int n, Grammar* pg);

