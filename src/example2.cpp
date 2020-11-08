#include <iostream>
#include "args.h"

using namespace args;
using namespace std;

void callback(string cmd_name, ArgParser& cmd_parser) {
    cout << "---------- boo! ----------\n";
    cmd_parser.print();
    cout << "--------------------------\n\n";
}

int main(int argc, char **argv) {
    ArgParser parser("Usage: example...", "1.0");

    ArgParser& cmd_parser = parser.command("boo", "Usage: example boo...", callback);
    cmd_parser.flag("foo f");
    cmd_parser.option("bar b", "default");

    parser.parse(argc, argv);
    parser.print();
}
