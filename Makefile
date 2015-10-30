# Makefile for insertion system simulation programs related to:
# B. Hescott, C. Malchik, A. Winslow, 
# "Tight bounds for active self-assembly using an insertion primitive",
# http://arxiv.org/abs/1401.0359

CC=clang
CPP=clang++
CFLAGS=-Wall

all: simulator pg2is g2pg fastgrowingpg highambiguitypg superfastgrowingis nondetermfastis 

# The main program that simulates insertion systems
simulator: simulator.cpp
	$(CPP) $(CFLAGS) simulator.cpp -o simulator

# Grammar and pair (symbol) grammar classes 
pairgrammar.o: pairgrammar.cpp pairgrammar.h
	$(CPP) $(CFLAGS) -c pairgrammar.cpp -o pairgrammar.o

grammar.o: grammar.cpp grammar.h pairgrammar.o 
	$(CPP) $(CFLAGS) -c grammar.cpp -o grammar.o

# Programs for converting grammars to pair grammars (g2pg) 
# and pair grammars to insertion systems (pg2is) as in the paper.
g2pg: g2pg.cpp grammar.o
	$(CPP) $(CFLAGS) -o g2pg g2pg.cpp grammar.o pairgrammar.o

pg2is: pg2is.cpp pairgrammar.o
	$(CPP) $(CFLAGS) -o pg2is pg2is.cpp pairgrammar.o

# Programs for generating instances of particular constructions.
highambiguitypg: highambiguitypg.c
	$(CC) $(CFLAGS) -o highambiguitypg highambiguitypg.c

fastgrowingpg: fastgrowingpg.cpp
	$(CPP) $(CFLAGS) -o fastgrowingpg fastgrowingpg.cpp

superfastgrowingis: superfastgrowingis.c
	$(CC) $(CFLAGS) -o superfastgrowingis superfastgrowingis.c

nondetermfastis: nondetermfastis.c
	$(CC) $(CFLAGS) -o nondetermfastis nondetermfastis.c

# Billy Mays  
clean:
	rm -f ./*.o
	rm -f ./simulator
	rm -f ./pg2is	
	rm -f ./g2pg
	rm -f ./fastgrowingpg
	rm -f ./highambiguity
	rm -f ./superfastgrowingis
	rm -f ./nondetermfastis
