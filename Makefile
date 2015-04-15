#CC=gcc-4.9
#CXX=g++-4.9
CC=gcc
CXX=g++
RM=rm -f
#CPPFLAGS=-Ofast -std=gnu++11 -fPIC -I ./include
CPPFLAGS=-Ofast -fPIC -I ./include
LDFLAGS=-Ofast
LDLIBS=

SRCS=src/pool.cc src/bracket.cc src/tournament.cc src/scorer.cc src/possibilities.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: pool

pool: $(OBJS)
	$(CXX) $(LDFLAGS) -o pool $(OBJS) $(LDLIBS)

#bracket.o: bracket.cc bracket.h
#scorer.o: scorer.cc scorer.h
#tournament.o: tournament.cc tournament.h
#pool.o: tournament.o bracket.o team.o scorer.o possibilities.o

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) pool
