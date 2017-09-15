// -----------------------------------------------------------------------------
// Optional: a minimalist argument-parsing library written in portable C++11.
// -----------------------------------------------------------------------------

#include "optional.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

using namespace std;

using opt::ArgStream;
using opt::OptionType;
using opt::Option;
using opt::ArgParser;


// -----------------------------------------------------------------------------
// String parsing utilities.
// -----------------------------------------------------------------------------


static int tryStringToInt(string arg) {
    try {
        return stoi(arg);
    } catch (invalid_argument) {
        cerr << "Error: cannot parse '" << arg << "' as an integer." << endl;
        exit(1);
    } catch (out_of_range) {
        cerr << "Error: " << arg << " is out of range." << endl;
        exit(1);
    }
}


static double tryStringToDouble(string arg) {
    try {
        return stod(arg);
    } catch (invalid_argument) {
        cerr << "Error: cannot parse '" << arg << "' ";
        cerr << "as a floating-point value." << endl;
        exit(1);
    } catch (out_of_range) {
        cerr << "Error: " << arg << " is out of range." << endl;
        exit(1);
    }
}


// -----------------------------------------------------------------------------
// Options.
// -----------------------------------------------------------------------------


void Option::trySetValue(string value) {
    switch (this->type) {
        case OptionType::String:
            this->strings.push_back(value);
            break;
        case OptionType::Int:
            this->ints.push_back(tryStringToInt(value));
            break;
        case OptionType::Double:
            this->doubles.push_back(tryStringToDouble(value));
            break;
        case OptionType::Flag:
            cerr << "Option::trySetValue: invalid code path." << endl;
            exit(1);
    }
}


// -----------------------------------------------------------------------------
// ArgStream.
// -----------------------------------------------------------------------------


void ArgStream::append(string arg) {
    args.push_back(arg);
}


string ArgStream::next() {
    string arg = args.front();
    args.pop_front();
    return arg;
}


bool ArgStream::hasNext() {
    return args.size() > 0;
}


// -----------------------------------------------------------------------------
// ArgParser: register options.
// -----------------------------------------------------------------------------


void ArgParser::newFlag(string name) {
    Option* option = new Option(OptionType::Flag);

    stringstream stream(name);
    string alias;

    while (stream >> alias) {
        options[alias] = option;
    }
}


void ArgParser::newString(string name, string fallback) {
    Option* option = new Option(OptionType::String);
    option->fb_string = fallback;

    stringstream stream(name);
    string alias;

    while (stream >> alias) {
        options[alias] = option;
    }
}


void ArgParser::newInt(string name, int fallback) {
    Option* option = new Option(OptionType::Int);
    option->fb_int = fallback;

    stringstream stream(name);
    string alias;

    while (stream >> alias) {
        options[alias] = option;
    }
}


void ArgParser::newDouble(string name, double fallback) {
    Option* option = new Option(OptionType::Double);
    option->fb_double = fallback;

    stringstream stream(name);
    string alias;

    while (stream >> alias) {
        options[alias] = option;
    }
}


// -----------------------------------------------------------------------------
// ArgParser: retrieve option values.
// -----------------------------------------------------------------------------


bool ArgParser::found(string name) {
    return options[name]->found;
}


bool ArgParser::getFlag(string name) {
    if (options[name]->bools.size() > 0) {
        return options[name]->bools.back();
    }
    return options[name]->fb_bool;
}


string ArgParser::getString(string name) {
    if (options[name]->strings.size() > 0) {
        return options[name]->strings.back();
    }
    return options[name]->fb_string;
}


int ArgParser::getInt(string name) {
    if (options[name]->ints.size() > 0) {
        return options[name]->ints.back();
    }
    return options[name]->fb_int;
}


double ArgParser::getDouble(string name) {
    if (options[name]->doubles.size() > 0) {
        return options[name]->doubles.back();
    }
    return options[name]->fb_double;
}


int ArgParser::lenList(string name) {
    switch (options[name]->type) {
        case OptionType::Flag:
            return options[name]->bools.size();
        case OptionType::String:
            return options[name]->strings.size();
        case OptionType::Int:
            return options[name]->ints.size();
        case OptionType::Double:
            return options[name]->doubles.size();
    }
}


vector<string> ArgParser::getStringList(string name) {
    return options[name]->strings;
}


vector<int> ArgParser::getIntList(string name) {
    return options[name]->ints;
}


vector<double> ArgParser::getDoubleList(string name) {
    return options[name]->doubles;
}


// -----------------------------------------------------------------------------
// ArgParser: retrieve positional arguments.
// -----------------------------------------------------------------------------


bool ArgParser::hasArgs() {
    return arguments.size() > 0;
}


int ArgParser::numArgs() {
    return arguments.size();
}


string ArgParser::getArg(int index) {
    return arguments[index];
}


vector<string> ArgParser::getArgs() {
    return arguments;
}


vector<int> ArgParser::getArgsAsInts() {
    vector<int> values;
    for (auto const & arg: arguments) {
        values.push_back(tryStringToInt(arg));
    }
    return values;
}


vector<double> ArgParser::getArgsAsDoubles() {
    vector<double> values;
    for (auto const & arg: arguments) {
        values.push_back(tryStringToDouble(arg));
    }
    return values;
}


// -----------------------------------------------------------------------------
// ArgParser: commands.
// -----------------------------------------------------------------------------


ArgParser& ArgParser::newCmd(
    string name, string helptext, void (*callback)(ArgParser& parser)) {

    ArgParser *parser = new ArgParser();
    parser->helptext = helptext;
    parser->parent = this;
    parser->callback = callback;

    stringstream stream(name);
    string alias;

    while (stream >> alias) {
        commands[alias] = parser;
    }

    return *parser;
}


bool ArgParser::hasCmd() {
    return command != "";
}


string ArgParser::getCmdName() {
    return command;
}


ArgParser& ArgParser::getCmdParser() {
    return *commands[command];
}


ArgParser& ArgParser::getParent() {
    return *parent;
}


bool ArgParser::hasParent() {
    return parent != nullptr;
}


// -----------------------------------------------------------------------------
// ArgParser: parse arguments.
// -----------------------------------------------------------------------------


// Parse an option of the form --name=value or -n=value.
void ArgParser::parseEqualsOption(string prefix, string name, string value) {

    // Do we have a registered option?
    auto const & element = options.find(name);
    if (element == options.end()) {
        cerr << "Error: " << prefix << name << " is not a recognised option.";
        cerr << endl;
        exit(1);
    }
    Option *opt = element->second;
    opt->found = true;

    // Boolean flags should never contain an equals sign.
    if (opt->type == OptionType::Flag) {
        cerr << "Error: invalid format for boolean flag ";
        cerr << prefix << name << "." << endl;
        exit(1);
    }

    // Make sure we have a value.
    if (value.size() == 0) {
        cerr << "Error: missing value for " << prefix << name << "." << endl;
        exit(1);
    }

    opt->trySetValue(value);
}


// Parse a long-form option, i.e. an option beginning with a double dash.
void ArgParser::parseLongOption(string arg, ArgStream& stream) {

    // Do we have an option of the form --name=value?
    size_t pos = arg.find("=");
    if (pos != string::npos) {
        this->parseEqualsOption("--", arg.substr(0, pos), arg.substr(pos + 1));
        return;
    }

    // Is the argument a registered option name?
    auto const & element = options.find(arg);
    if (element != options.end()) {
        Option *opt = element->second;
        opt->found = true;
        if (opt->type == OptionType::Flag) {
            opt->bools.push_back(true);
        } else if (stream.hasNext()) {
            opt->trySetValue(stream.next());
        } else {
            cerr << "Error: missing argument for --" << arg << "." << endl;
            exit(1);
        }
        return;
    }

    // Is the argument an automatic --help flag?
    if (arg == "help" && this->helptext != "") {
        this->exitHelp();
    }

    // Is the argument an automatic --version flag?
    if (arg == "version" && this->version != "") {
        this->exitVersion();
    }

    // The argument is not a registered or automatic option name.
    cerr << "Error: --" << arg << " is not a recognised option." << endl;
    exit(1);
}


// Parse a short-form option, i.e. an option beginning with a single dash.
void ArgParser::parseShortOption(string arg, ArgStream& stream) {

    // Do we have an option of the form -n=value?
    size_t pos = arg.find("=");
    if (pos != string::npos) {
        this->parseEqualsOption("-", arg.substr(0, pos), arg.substr(pos + 1));
        return;
    }

    // We examine each character individually to support condensed options
    // with trailing arguments: -abc foo bar. If we don't recognise the
    // character as a registered option name, we check for an automatic
    // -h or -v flag before exiting.
    for (char& c: arg) {
        auto const & element = options.find(string(1, c));
        if (element == options.end()) {
            if (c == 'h' && this->helptext != "") {
                this->exitHelp();
            } else if (c == 'v' && this->version != "") {
                this->exitVersion();
            } else {
                cerr << "Error: -" << c << " is not a recognised option.";
                cerr << endl;
                exit(1);
            }
        }

        Option *opt = element->second;
        opt->found = true;

        if (opt->type == OptionType::Flag) {
            opt->bools.push_back(true);
        } else if (stream.hasNext()) {
            opt->trySetValue(stream.next());
        } else {
            cerr << "Error: missing argument for -" << c << "." << endl;
            exit(1);
        }
    }
}


// Parse a stream of string arguments.
void ArgParser::parse(ArgStream& stream) {
    bool parsing = true;
    bool is_first_arg = true;

    // Loop while we have arguments to process.
    while (stream.hasNext()) {
        string arg = stream.next();

        // If parsing has been turned off, simply add the argument to the
        // list of positionals.
        if (!parsing) {
            arguments.push_back(arg);
            continue;
        }

        // If we enounter a '--', turn off option parsing.
        if (arg == "--") {
            parsing = false;
            continue;
        }

        // Is the argument a long-form option or flag?
        if (arg.compare(0, 2, "--") == 0) {
            this->parseLongOption(arg.substr(2), stream);
            continue;
        }

        // Is the argument a short-form option or flag? If the argument
        // consists of a single dash or a dash followed by a digit, we treat
        // it as a positional argument.
        if (arg[0] == '-') {
            if (arg.size() == 1 || isdigit(arg[1])) {
                arguments.push_back(arg);
            } else {
                this->parseShortOption(arg.substr(1), stream);
            }
            continue;
        }

        // Is the argument a registered command?
        auto element = commands.find(arg);
        if (element != commands.end()) {
            ArgParser *cmdparser = element->second;
            this->command = arg;
            cmdparser->parse(stream);
            cmdparser->callback(*cmdparser);
            continue;
        }

        // Is the argument the automatic 'help' command?
        if (is_first_arg && arg == "help") {
            if (stream.hasNext()) {
                string name = stream.next();
                if (commands.find(name) == commands.end()) {
                    cerr << "Error: '" << name;
                    cerr << "' is not a recognised command" << endl;
                    exit(1);
                } else {
                    commands[name]->exitHelp();
                }
            } else {
                cerr << "Error: the help command requires an argument.";
                cerr << endl;
                exit(1);
            }
        }

        // Otherwise add the argument to our list of positional arguments.
        this->arguments.push_back(arg);
        is_first_arg = false;
    }
}


// Parse an array of string arguments. We assume that argc and argv are the
// original parameters passed to main() and skip the first element.
void ArgParser::parse(int argc, char **argv) {
    ArgStream stream;
    for (int i = 1; i < argc; i++) {
        stream.append(argv[i]);
    }
    this->parse(stream);
}


// -----------------------------------------------------------------------------
// ArgParser: utilities.
// -----------------------------------------------------------------------------


// Override the << stream insertion operator to support vectors. This will
// allow us to cout our lists of option values in the print() method.
template<typename T>
static ostream& operator<<(ostream& stream, const vector<T>& vec) {
    stream << "[";
    for(size_t i = 0; i < vec.size(); ++i) {
        if (i) cout << ", ";
        stream << vec[i];
    }
    stream << "]";
    return stream;
}


// Print the parser instance to standard out.
void ArgParser::print() {
    cout << "Options:" << endl;
    if (options.size() > 0) {
        for (auto element: options) {
            cout << "  " << element.first << ": ";
            Option *option = element.second;
            switch (option->type) {
                case OptionType::Flag:
                    cout << "(" << option->fb_bool << ") ";
                    cout << option->bools;
                    break;
                case OptionType::String:
                    cout << "(" << option->fb_string << ") ";
                    cout << option->strings;
                    break;
                case OptionType::Int:
                    cout << "(" << option->fb_int << ") ";
                    cout << option->ints;
                    break;
                case OptionType::Double:
                    cout << "(" << option->fb_double << ") ";
                    cout << option->doubles;
                    break;
            }
            cout << endl;
        }
    } else {
        cout << "  [none]" << endl;
    }

    cout << endl << "Arguments:" << endl;
    if (arguments.size() > 0) {
        for (auto arg: arguments) {
            cout << "  " << arg << endl;
        }
    } else {
        cout << "  [none]" << endl;
    }

    cout << endl << "Command:" << endl;
    if (this->hasCmd()) {
        cout << "  " << command << endl;
    } else {
        cout << "  [none]" << endl;
    }
}


// Print the parser's help text and exit.
void ArgParser::exitHelp() {
    cout << this->helptext << endl;
    exit(0);
}


// Print the parser's version string and exit.
void ArgParser::exitVersion() {
    cout << this->version << endl;
    exit(0);
}


// -----------------------------------------------------------------------------
// ArgParser: cleanup.
// -----------------------------------------------------------------------------


ArgParser::~ArgParser() {

    // Free the memory occupied by registered Option instances. Each
    // instance can be registered under multiple names.
    vector<Option*> odel;
    for (auto element: options) {
        Option *optionptr = element.second;
        if (find(odel.begin(), odel.end(), optionptr) == odel.end()) {
            delete optionptr;
            odel.push_back(optionptr);
        }
    }

    // Free the memory occupied by registered command parsers. Each parser
    // instance can be registered under multiple names.
    vector<ArgParser*> pdel;
    for (auto element: commands) {
        ArgParser *parserptr = element.second;
        if (find(pdel.begin(), pdel.end(), parserptr) == pdel.end()) {
            delete parserptr;
            pdel.push_back(parserptr);
        }
    }
}
