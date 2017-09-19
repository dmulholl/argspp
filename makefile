# --------------------------------------------------------------------------
# Make variables.
# --------------------------------------------------------------------------

CFLAGS = -Wall -Wextra -Wno-unused-parameter --stdlib=libc++ --std=c++11

# --------------------------------------------------------------------------
# Targets.
# --------------------------------------------------------------------------

example: src/example.cpp src/options.cpp src/options.h
	@mkdir -p bin
	c++ $(CFLAGS) -o bin/example src/example.cpp src/options.cpp

clean:
	rm -f ./src/*.o
	rm -f ./bin/example
