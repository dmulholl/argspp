// -----------------------------------------------------------------------------
// Unit test suite.
// -----------------------------------------------------------------------------

#include <cassert>
#include <vector>
#include <string>
#include "args.h"

using namespace std;
using namespace args;

// -----------------------------------------------------------------------------
// 1. Flags.
// -----------------------------------------------------------------------------

void test_flag_empty_input() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>());
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    printf(".");
}

void test_flag_not_found() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"abc", "def"}));
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    printf(".");
}

void test_flag_long() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"--foo"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    printf(".");
}

void test_flag_short() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-f"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    printf(".");
}

void test_flag_condensed() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-fff"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 3);
    printf(".");
}

void test_flag_multi() {
    ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-fff", "--foo", "-f"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 5);
    printf(".");
}

// -----------------------------------------------------------------------------
// 2. Options.
// -----------------------------------------------------------------------------

void test_option_not_found() {
    ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"abc", "def"}));
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    assert(parser.value("foo") == "default");
    printf(".");
}

void test_option_long() {
    ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"--foo", "bar"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    assert(parser.value("foo") == "bar");
    printf(".");
}

void test_option_short() {
    ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"-f", "bar"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    assert(parser.value("foo") == "bar");
    printf(".");
}

void test_option_condensed() {
    ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"-ff", "bar", "baz"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 2);
    assert(parser.value("foo") == "baz");
    printf(".");
}

void test_option_multi() {
    ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"-ff", "bar", "baz", "--foo", "bam"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 3);
    assert(parser.value("foo") == "bam");
    printf(".");
}

// -----------------------------------------------------------------------------
// 3. Positional arguments.
// -----------------------------------------------------------------------------

void test_pos_args() {
    ArgParser parser;
    parser.parse(vector<string>({"abc", "def"}));
    assert(parser.args.size() == 2);
    assert(parser.args[0] == "abc");
    assert(parser.args[1] == "def");
    printf(".");
}

// -----------------------------------------------------------------------------
// 4. Option parsing switch.
// -----------------------------------------------------------------------------

void test_option_parsing_switch() {
    ArgParser parser;
    parser.parse(vector<string>({"foo", "--", "--bar", "--baz"}));
    assert(parser.args.size() == 3);
    printf(".");
}

// -----------------------------------------------------------------------------
// 7. Commands.
// -----------------------------------------------------------------------------

void test_command() {
    ArgParser parser;
    ArgParser& cmd_parser = parser.command("boo");
    cmd_parser.flag("foo");
    cmd_parser.option("bar", "default");
    parser.parse(vector<string>({"boo", "abc", "def", "--foo", "--bar", "baz"}));
    assert(parser.commandFound());
    assert(parser.commandName() == "boo");
    assert(cmd_parser.args.size() == 2);
    assert(cmd_parser.found("foo"));
    assert(cmd_parser.value("bar") == "baz");
    printf(".");
}

// -----------------------------------------------------------------------------
// Test runner.
// -----------------------------------------------------------------------------

void line() {
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n");
}

int main() {
    setbuf(stdout, NULL);
    line();

    printf("Tests: 1 ");
    test_flag_empty_input();
    test_flag_not_found();
    test_flag_long();
    test_flag_short();
    test_flag_condensed();
    test_flag_multi();

    printf(" 2 ");
    test_option_not_found();
    test_option_long();
    test_option_short();
    test_option_condensed();
    test_option_multi();

    printf(" 3 ");
    test_pos_args();

    printf(" 4 ");
    test_option_parsing_switch();

    printf(" 5 ");
    test_command();

    printf(" [ok]\n");
    line();
}
