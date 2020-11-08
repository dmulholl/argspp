// -----------------------------------------------------------------------------
// Unit test suite.
// -----------------------------------------------------------------------------

#include <cassert>
#include <vector>
#include <string>
#include "args.h"

using namespace std;

// -----------------------------------------------------------------------------
// 1. Flags.
// -----------------------------------------------------------------------------

void test_flag_empty() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>());
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    printf(".");
}

void test_flag_missing() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"abc", "def"}));
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    printf(".");
}

void test_flag_long() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"--foo"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    printf(".");
}

void test_flag_short() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-f"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    printf(".");
}

void test_flag_condensed() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-fff"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 3);
    printf(".");
}

void test_flag_multi() {
    args::ArgParser parser;
    parser.flag("foo f");
    parser.parse(vector<string>({"-fff", "--foo", "-f"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 5);
    printf(".");
}

// -----------------------------------------------------------------------------
// 2. Options.
// -----------------------------------------------------------------------------

void test_opt_default() {
    args::ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"abc", "def"}));
    assert(parser.found("foo") == false);
    assert(parser.count("foo") == 0);
    assert(parser.value("foo") == "default");
    printf(".");
}

void test_opt_long() {
    args::ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"--foo", "bar"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    assert(parser.value("foo") == "bar");
    printf(".");
}

void test_opt_short() {
    args::ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"-f", "bar"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 1);
    assert(parser.value("foo") == "bar");
    printf(".");
}

void test_opt_condensed() {
    args::ArgParser parser;
    parser.option("foo f", "default");
    parser.parse(vector<string>({"-ff", "bar", "baz"}));
    assert(parser.found("foo") == true);
    assert(parser.count("foo") == 2);
    assert(parser.value("foo") == "baz");
    printf(".");
}

void test_opt_multi() {
    args::ArgParser parser;
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
    args::ArgParser parser;
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
    args::ArgParser parser;
    parser.parse(vector<string>({"foo", "--", "--bar", "--baz"}));
    assert(parser.args.size() == 3);
    printf(".");
}

// -----------------------------------------------------------------------------
// 7. Commands.
// -----------------------------------------------------------------------------

void test_command() {
    args::ArgParser parser;
    args::ArgParser& cmd_parser = parser.command("boo");
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
    test_flag_empty();
    test_flag_missing();
    test_flag_long();
    test_flag_short();
    test_flag_condensed();
    test_flag_multi();

    printf(" 2 ");
    test_opt_default();
    test_opt_long();
    test_opt_short();
    test_opt_condensed();
    test_opt_multi();

    printf(" 3 ");
    test_pos_args();

    printf(" 4 ");
    test_option_parsing_switch();

    printf(" 5 ");
    test_command();

    printf(" [ok]\n");
    line();
}
