#CC=gcc-4.9
#CXX=g++-4.9
CC=gcc
CXX=g++
RM=rm -f
#CPPFLAGS=-Ofast -std=gnu++11 -fPIC -I ./include
CPPFLAGS=-Ofast -fPIC -I ./include
<<<<<<< HEAD
LDFLAGS=-Ofast -L. -lpool
LDLIBS=
=======
LDFLAGS=-Ofast -L.
LDLIBS=-lpool
>>>>>>> d719525 (minor bug fixes, allow tracking entry by name)
LIBFILE=libpool.a

LIBSRCS=src/bracket.cc src/tournament.cc src/scorer.cc src/possibilities.cc src/stats.cc
LIBOBJS=$(subst .cc,.o,$(LIBSRCS))

SRCS=src/pool.cc
OBJS=$(subst .cc,.o,$(SRCS))

TESTSRCS=src/test.cc
TESTOBJS=$(subst .cc,.o,$(TESTSRCS))

all: pool test

lib: $(LIBOBJS)
	ar ru $(LIBFILE) $(LIBOBJS)

pool: $(OBJS) lib
	$(CXX) $(LDFLAGS) -o pool $(OBJS) $(LDLIBS)

test: $(TESTOBJS) lib
	$(CXX) $(LDFLAGS) -o test $(TESTOBJS) $(LDLIBS)

#bracket.o: bracket.cc bracket.h
#scorer.o: scorer.cc scorer.h
#tournament.o: tournament.cc tournament.h
#pool.o: tournament.o bracket.o team.o scorer.o possibilities.o

clean:
	$(RM) $(OBJS) $(LIBOBJS) $(TESTOBJS) $(LIBFILE) pool test

dist-clean: clean
	$(RM) pool
