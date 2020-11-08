# ------------------------------------------------------------------------------
# Make variables.
# ------------------------------------------------------------------------------

CFLAGS = -Wall -Wextra -Wno-unused-parameter --stdlib=libc++ --std=c++11

# ------------------------------------------------------------------------------
# Targets.
# ------------------------------------------------------------------------------

examples: src/example1.cpp src/example2.cpp src/args.cpp src/args.h
	@mkdir -p bin
	c++ $(CFLAGS) -o bin/example1 src/example1.cpp src/args.cpp
	c++ $(CFLAGS) -o bin/example2 src/example2.cpp src/args.cpp

clean:
	rm -f ./src/*.o
	rm -f ./bin/example1
	rm -f ./bin/example2
