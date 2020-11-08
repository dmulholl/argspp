#include "args.h"

int main(int argc, char **argv) {
    args::ArgParser parser("Usage: example...", "1.0");

    parser.flag("foo f");
    parser.option("bar b", "default");

    parser.parse(argc, argv);
    parser.print();
}
