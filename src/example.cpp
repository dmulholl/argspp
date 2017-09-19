// -----------------------------------------------------------------------------
// A simple application demonstrating Options in action.
// -----------------------------------------------------------------------------

#include <iostream>
#include "options.h"

using namespace std;


// Callback function for the 'foo' command. This function will be called if the
// command is found. The function receives a reference to an ArgParser instance
// containing the command's parsed arguments. Here we simply dump the parser
// to stdout.
void callback(opt::ArgParser& parser) {
    cout << "---------- callback ----------\n";
    parser.print();
    cout << "------------------------------\n\n";
}


// This sample application will parse its own command-line arguments.
int main(int argc, char **argv) {

    // We instantiate an argument parser, optionally supplying help text and
    // a version string. Supplying help text activates an automatic --help
    // flag, supplying a version string activates an automatic --version
    // flag. We can pass an empty string for either parameter.
    opt::ArgParser parser("Help!", "1.2.3");

    // Register a flag, --bool, with a single-character alias, -b. A flag is a
    // boolean option - it's either present (true) or absent (false).
    parser.newFlag("bool b");

    // Register a string option, --string <arg>, with a single-character
    // alias, -s <arg>. The default fallback value for string options is the
    // empty string. Here we specify a custom fallback value, "foobar".
    parser.newString("string s", "foobar");

    // Register an integer option, --int <arg>, with a single-character alias,
    // -i <arg>. The default fallback value for integer options is 0.
    parser.newInt("int i");

    // Register a floating-point option, --double <arg>, with a single-
    // character alias, -d <arg>. The default fallback value for floating-point
    // options is 0.
    parser.newDouble("double d");

    // Register a command 'foo'. We need to supply the command's help text and
    // callback function.
    opt::ArgParser& cmd = parser.newCmd("foo", "Command!", callback);

    // Registering a command returns a new ArgParser instance dedicated to
    // parsing the command's arguments. We can register as many flags and
    // options as we like on this sub-parser. Note that the sub-parser can
    // reuse the parent's option names without interference.
    cmd.newFlag("bool b");
    cmd.newInt("int i", 123);

    // Once all our options and commands have been registered we can call the
    // parse() method with an array of argument strings. (Note that we
    // only need to call parse() on the root parser - command arguments
    // are parsed automatically.)
    parser.parse(argc, argv);

    // In a real application we could now retrieve our option and argument
    // values from the parser instance. Here we simply dump the parser to
    // stdout.
    parser.print();
}
