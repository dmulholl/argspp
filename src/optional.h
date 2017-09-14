// -----------------------------------------------------------------------------
// Optional: a minimalist argument-parsing library written in portable C++11.
//
// Author: Darren Mulholland <darren@mulholland.xyz>
// License: Public Domain
// Version: 0.1.0
// -----------------------------------------------------------------------------

#pragma once

#include <deque>
#include <map>
#include <string>
#include <vector>

namespace opt {

    // Callback type for registering commands.
    class ArgParser;
    typedef void (*CommandCB)(ArgParser& parser);

    // Internal use.
    class ArgStream {
        public:
            void append(std::string arg);
            std::string next();
            bool hasNext();
        private:
            std::deque<std::string> args;
    };

    // Internal use.
    enum class OptionType {
        Flag, String, Int, Double
    };

    // Internal use.
    class Option {
        public:
            OptionType type;
            std::vector<bool> bools;
            std::vector<std::string> strings;
            std::vector<int> ints;
            std::vector<double> doubles;
            bool fb_bool = false;
            std::string fb_string;
            int fb_int = 0;
            double fb_double = 0.0;
            bool found = false;

            Option(OptionType type)
                : type(type) {}

            void trySetValue(std::string value);
    };

    // ArgParser provides the external interface to the library.
    class ArgParser {

        public:

            // Application help text and version string.
            std::string helptext;
            std::string version;

            ArgParser(std::string helptext = "", std::string version = "")
                : helptext(helptext), version(version) {}

            ~ArgParser();

            // Register options.
            void newFlag(std::string name);
            void newString(std::string name, std::string fallback = "");
            void newInt(std::string name, int fallback = 0);
            void newDouble(std::string name, double fallback = 0.0);

            // Parse command line arguments.
            void parse(int argc, char **argv);
            void parse(ArgStream& args);

            // Returns true if the named option was found while parsing.
            bool found(std::string name);

            // Fetch option values.
            bool getFlag(std::string name);
            std::string getString(std::string name);
            int getInt(std::string name);
            double getDouble(std::string name);

            // Returns the length of the named option's list of values.
            int lenList(std::string name);

            // Fetch list option values.
            std::vector<bool> getFlagList(std::string name);
            std::vector<std::string> getStringList(std::string name);
            std::vector<int> getIntList(std::string name);
            std::vector<double> getDoubleList(std::string name);

            // Fetch positional arguments.
            bool hasArgs();
            int numArgs();
            std::string getArg(int index);
            std::vector<std::string> getArgs();
            std::vector<int> getArgsAsInts();
            std::vector<double> getArgsAsFloats();

            // Register a command.
            ArgParser& newCmd(
                std::string name,
                std::string help,
                CommandCB cb
            );

            // Utilities for handling commands manually.
            bool hasCmd();
            std::string getCmdName();
            ArgParser& getCmdParser();
            ArgParser& getParent();
            bool hasParent();

            // Print a parser instance to stdout.
            void print();

            // Print the help text or version string and exit.
            void exitHelp();
            void exitVersion();

        private:

            std::map<std::string, Option*> options;
            std::map<std::string, ArgParser*> commands;
            std::vector<std::string> arguments;
            std::string command;
            ArgParser *parent = nullptr;
            CommandCB callback;

            void parseLongOption(std::string arg, ArgStream& stream);
            void parseShortOption(std::string arg, ArgStream& stream);
            void parseEqualsOption(
                std::string prefix,
                std::string name,
                std::string value
            );
    };
}
