# ------------------------------------------------------------------------------
# Make variables.
# ------------------------------------------------------------------------------

CXXFLAGS = -Wall -Wextra -Wno-unused-parameter --stdlib=libc++ --std=c++11

# ------------------------------------------------------------------------------
# Phony targets.
# ------------------------------------------------------------------------------

all::
	@make lib
	@make ex1
	@make ex2
	@make tests

lib::
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -c -o bin/args.o src/args.cpp

ex1::
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/ex1 src/example1.cpp src/args.cpp

ex2::
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/ex2 src/example2.cpp src/args.cpp

tests::
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/tests src/tests.cpp src/args.cpp

check::
	@make tests
	./bin/tests

clean::
	rm -f ./bin/*
