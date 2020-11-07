// -----------------------------------------------------------------------------
// Args++: an argument-parsing library in portable C++11.
//
// Author: Darren Mulholland <dmulholl@tcd.ie>
// License: Public Domain
// Version: 0.9.0
// -----------------------------------------------------------------------------

#ifndef args_h
#define args_h

#include <map>
#include <string>
#include <vector>

namespace args {

    // Forward declarations.
    class ArgStream;
    class Option;

    // ArgParser provides the public interface to the library.
    class ArgParser {
        public:

            // Application help text and version string.
            std::string helptext;
            std::string version;

            ArgParser(
                std::string const& helptext = "",
                std::string const& version = ""
            ) : helptext(helptext), version(version) {}

            ~ArgParser();

            // Register options.
            void newFlag(std::string const& name);
            void newDouble(std::string const& name, double fallback = 0.0);
            void newInt(std::string const& name, int fallback = 0);
            void newString(
                std::string const& name,
                std::string const& fallback = ""
            );

            // Parse command line arguments.
            void parse(int argc, char **argv);

            // Returns true if the named option was foundg.
            bool found(std::string const& name);

            // Returns the number of times the named option was found.
            int count(std::string const& name);

            // Retrieve option values.
            double getDouble(std::string const& name);
            bool getFlag(std::string const& name);
            int getInt(std::string const& name);
            std::string getString(std::string const& name);

            // Deprecated: replaced by count().
            int lenList(std::string const& name);

            // Retrieve list option values.
            std::vector<double> getDoubleList(std::string const& name);
            std::vector<int> getIntList(std::string const& name);
            std::vector<std::string> getStringList(std::string const& name);

            // Retrieve positional arguments.
            bool hasArgs();
            int numArgs();
            std::string getArg(int index);
            std::vector<std::string> getArgs();
            std::vector<int> getArgsAsInts();
            std::vector<double> getArgsAsDoubles();

            // Register a command.
            ArgParser& newCmd(
                std::string const& name,
                std::string const& helptext,
                void (*callback)(ArgParser& parser)
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
            void (*callback)(ArgParser& parser);

            void parse(ArgStream& args);
            void registerOption(std::string const& name, Option* option);
            void parseLongOption(std::string arg, ArgStream& stream);
            void parseShortOption(std::string arg, ArgStream& stream);
            void parseEqualsOption(
                std::string prefix,
                std::string name,
                std::string value
            );
    };
}

#endif
