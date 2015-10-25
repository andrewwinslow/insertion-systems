#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <climits>

using std::cin;
using std::cout;
using std::cerr;
using std::list;
using std::string;
using std::stack;
using std::vector;

int max = 0;

typedef struct
{
	int a, b, c, d;
	char p;
} MonomerType;

typedef struct Monomer Monomer;

struct Monomer 
{
	MonomerType type;
	Monomer* prev;
	Monomer* next;
};

typedef struct
{
	Monomer* monomer;
	int type;
} Insertion;




static vector<MonomerType> monomer_types;
static Monomer* polymer;
static int polymer_size;
static stack<Insertion> insertions; 
static bool sflag = false; /* size flag (just print polymer sizes) */
static bool vflag = false; /* verbose flag (print each insertion) */

void print_monomer(MonomerType monomer, bool sign);

// inserts after
void insert_monomer(MonomerType t, Monomer* loc)
{
	if(vflag) {
		cout << "inserting ";
		print_monomer(t, true);
		cout << " into ";
		print_monomer(loc->type, false);
		print_monomer(loc->next->type, false);
		cout << std::endl;
	}

	Monomer* new_monomer = (Monomer*) malloc(sizeof(Monomer));

	new_monomer->next = loc->next;
	loc->next->prev = new_monomer;
	loc->next = new_monomer;
	new_monomer->prev = loc;
	new_monomer->type = t;

	++polymer_size;
}

void remove_monomer(Monomer* mon)
{
	Monomer* prev = mon->prev;
	Monomer* next = mon->next;
	prev->next = next;
	next->prev = prev;

	free(mon);

	--polymer_size;
}

bool monomers_equal(MonomerType m1, MonomerType m2)
{
	return (m1.a == m2.a && m1.b == m2.b && m1.c == m2.c && m1.d == m2.d && m1.p == m2.p);
}

int abs(int n)
{
	return n < 0 ? -n : n;
}

int desanitize(int n)
{
	if (n == INT_MAX || n == -INT_MAX)
		return 0;
	return abs(n);
}

int sanitize(int n, bool c)
{
	if (n != 0)
		return c ? -n : n;
	return (c ? -INT_MAX : INT_MAX);
}

void print_monomer_rh(MonomerType monomer)
{
	cout << "(" << desanitize(monomer.c) << (monomer.c < 0 ? "*, " : ", ") << desanitize(monomer.d) << (monomer.d < 0 ? "*" : "") << ")"; 
}

void print_monomer_lh(MonomerType monomer)
{
	cout << "(" << desanitize(monomer.a) << (monomer.a < 0 ? "*, " : ", ") << desanitize(monomer.b) << (monomer.b < 0 ? "*" : "") << ")"; 
}

void print_monomer(MonomerType monomer, bool sign)
{
	if(monomer.p == 'l')
		return print_monomer_rh(monomer); // naming: left hand init monomer is printed by
	if(monomer.p == 'r')                  //         printing its right two symbols, so
		return print_monomer_lh(monomer); //         currently rh is called, etc.

	cout << "(" << desanitize(monomer.a) << (monomer.a < 0 ? "*" : "") << ", " 
		<< desanitize(monomer.b) << (monomer.b < 0 ? "*" : "") << ", "
		<< desanitize(monomer.c) << (monomer.c < 0 ? "*" : "") << ", "
		<< desanitize(monomer.d) << (monomer.d < 0 ? "*" : "") << ")"
		<< (sign ? (monomer.p == '+' ? "+" : "-") : "");
}

bool insertable(MonomerType inserted, Monomer* loc)
{
	MonomerType left_mon = loc->type;
	MonomerType right_mon = loc->next->type;

	if (inserted.p == '+')
		return (left_mon.d == -inserted.a) && (-inserted.d == right_mon.a) && (left_mon.c == -right_mon.b); 
	if (inserted.p == '-')
		return (left_mon.c == -inserted.b) && (-inserted.c == right_mon.b) && (left_mon.d == -right_mon.a); 
	return false;
}

void print_polymer()
{
	if(sflag) {
		if (polymer_size > max) {
			cout << "Polymer size: " << polymer_size << std::endl; 
			max = polymer_size;
		}
		return;
	}

	Monomer* cur = polymer;
	while (cur != NULL)
	{

		if (cur->prev == NULL)
			print_monomer_rh(cur->type);
		else if (cur->next == NULL)
			print_monomer_lh(cur->type);
		else
			print_monomer(cur->type, false);
		cout << ' ';
		cur = cur->next;
	}

	cout << std::endl;
}		

void simulate()
{
	Insertion insert;
	Monomer* site = polymer;
	int type = 0;
	bool site_insertable = false;

	while (insertions.size() != 0 || type < monomer_types.size())
	{	
		// if you've reached the end
		if (site->next == NULL)
		{
			if(vflag)
				cout << "terminal polymer: \n";
			// print the polymer and pop the stack
			print_polymer();
			if(vflag)
				cout << "------------------------------\n";
			site = insertions.top().monomer->prev;
			type = insertions.top().type+1;
			remove_monomer(insertions.top().monomer);
			insertions.pop();
			site_insertable = true;
			continue;
		}
		
		// roll over to next site
		if (type == monomer_types.size())
		{
			if (site_insertable)
			{
				// pop the stack
				site = insertions.top().monomer->prev;
				type = insertions.top().type+1;
				remove_monomer(insertions.top().monomer);
				insertions.pop();					
				site_insertable = true;
			}
			else
			{
				// continue on to the next site
				type = 0;
				site = site->next;	
				site_insertable = false;
			}
			continue;
		}

		// the usual case: try to insert the monomer,
		// if insertion isn't possible, go to the next monomer
		if (insertable(monomer_types[type], site))
		{
			insert_monomer(monomer_types[type], site);
			insert.monomer = site->next;
			insert.type = type;
			insertions.push(insert);

			type = 0;
			site_insertable = false;
		}
		else
			++type;	
	}
}
   
 
int main(int argc, char *argv[])
{
	polymer_size = 0;

	MonomerType m;
	int n;
	bool c;

	// thanks K&R (1988 p.106)
	char a;
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((a = *++argv[0]))
			switch (a) {
			case 's':
				sflag = true;
				break;
			case 'v':
				vflag = true;
				break;
			default:
				cout << "illegal option " << a << std::endl;
				break;
			}

	while (cin)
	{
		cin >> std::ws;
		if (cin.peek() == '#')
		{
			cin.ignore(1000, '\n');
			continue;
		}
		if (cin.peek() == EOF)
			break;
	
		if (cin.peek() != '(' || !(cin.ignore()))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '('.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		if (!(cin >> n))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected an integer.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		c = (cin.peek() == '*');
		if (cin.peek() == '*')
		{
			cin.ignore();
			cin >> std::ws;
		}
		m.a = sanitize(n, c);

		if (cin.peek() != ',' || !(cin.ignore()))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '*' or ','.\n";
			return EXIT_FAILURE;	
		}
		cin >> std::ws;
		if (!(cin >> n))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected an integer.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		c = (cin.peek() == '*');
		if (cin.peek() == '*')
		{
			cin.ignore();
			cin >> std::ws;
		}
		m.b = sanitize(n, c);

		if (cin.peek() == ')')
		{
			cin.ignore();
			if (polymer_size == 0)
			{
				polymer = (Monomer*) malloc(sizeof(Monomer));
				polymer->prev = polymer->next = NULL;
				++polymer_size;
				m.c = m.a;
				m.d = m.b;
				m.a = m.b = 0;
				m.p = 'l'; /* left initiator monomer */
				polymer->type = m;
			}
			else if (polymer_size == 1)
			{
				polymer->next = (Monomer*) malloc(sizeof(Monomer));
				polymer->next->prev = polymer;
				polymer->next->next = NULL;
				++polymer_size;
				m.c = m.d = 0;
				m.p = 'r'; /* right initiator monomer */
				polymer->next->type = m;
			}
			else if (polymer_size == 2)
			{
				cerr << "Error: more than two initiator halves specified.\n";
				return EXIT_FAILURE;
			}
			continue;
		}
		if (cin.peek() != ',' || !(cin.ignore()))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '*', ',', or ')'.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;

		if (!(cin >> n))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected an integer.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		c = (cin.peek() == '*');
		if (cin.peek() == '*')
		{
			cin.ignore();
			cin >> std::ws;
		}
		m.c = sanitize(n, c);

		if (cin.peek() != ',' || !(cin.ignore()))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '*' or ','.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		if (!(cin >> n))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected an integer.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		c = (cin.peek() == '*');
		if (cin.peek() == '*')
		{
			cin.ignore();
			cin >> std::ws;
		}
		m.d = sanitize(n, c);

		if (cin.peek() != ')' || !(cin.ignore()))
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '*' or ')'.\n";
			return EXIT_FAILURE;
		}
		cin >> std::ws;
		if (cin.peek() != '+' && cin.peek() != '-')
		{
			cerr << "Error: unexpected token '" << cin.peek() << "', expected '+' or '-'.\n";
			return EXIT_FAILURE;
		}
		cin >> m.p;

		bool repeat = false;
		for (int i = 0; i < monomer_types.size(); ++i)
			if (monomers_equal(m, monomer_types[i]))
			{
				repeat = true;
			}
		if (!repeat)
			monomer_types.push_back(m);
	}

	simulate();

	return EXIT_SUCCESS;
}




