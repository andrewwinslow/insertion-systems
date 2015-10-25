
CC=clang
CPP=clang++
CFLAGS=-Wall

all: simulator pg2is g2pg fastgrowingpg highambiguitypg superfastgrowingis nondetermfastis 

# The main program that simulates insertion systems
simulator: simulator.cpp
	$(CPP) $(CFLAGS) simulator.cpp -o simulator

# Grammar and pair (symbol) grammar classes 
grammar.o: grammar.c grammar.h
	$(CC) $(CFLAGS) -c grammar.c -o grammar.o

pairgrammar.o: pairgrammar.c pairgrammar.h
	$(CC) $(CFLAGS) -c pairgrammar.c -o pairgrammar.o

# Programs for converting grammars to pair grammars (g2pg) 
# and pair grammars to insertion systems (pg2is) as in the paper.
g2pg: g2pg.c grammar.o
	$(CC) $(CFLAGS) -o g2pg g2pg.c grammar.o

pg2is: pg2is.c pairgrammar.o
	$(CC) $(CFLAGS) -o pg2is pg2is.c pairgrammar.o

# Programs for generating instances of particular constructions.
highambiguitypg: highambiguitypg.c
	$(CC) $(CFLAGS) -o highambiguitypg highambiguitypg.c

fastgrowingpg: fastgrowingpg.c
	$(CC) $(CFLAGS) -o fastgrowingpg fastgrowingpg.c

superfastgrowingis: superfastgrowingis.c
	$(CC) $(CFLAGS) -o superfastgrowingis superfastgrowingis.c

nondetermfastis: nondetermfastis.c
	$(CC) $(CFLAGS) -o nondetermfastis nondetermfastis.c

# Billy Mays  
clean:
	rm -f ./*.o
	rm -f ./simulator
	rm -f ./generator
	rm -f ./pg2is	
	rm -f ./g2pg
	rm -f ./fastgrowingpg
	rm -f ./highambiguity
	rm -f ./superfastgrowingis
	rm -f ./nondetermfastis
