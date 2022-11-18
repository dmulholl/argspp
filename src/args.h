// -----------------------------------------------------------------------------
// Args++: an argument-parsing library in portable C++11.
//
// Author: Darren Mulholland <dmulholl@tcd.ie>
// License: Public Domain
// Version: 2.1.0
// -----------------------------------------------------------------------------

#ifndef args_h
#define args_h

#include <map>
#include <string>
#include <vector>
#include <sstream>

namespace args {

    struct ArgStream;
    struct Option;
    struct Flag;

    class ArgParser {
        public:
            ArgParser(
                std::string const& helptext = "",
                std::string const& version = ""
            ) : helptext(helptext), version(version) {}

            ~ArgParser();

            // Stores positional arguments.
            std::vector<std::string> args;

            // Application/command help text and version strings.
            std::string helptext;
            std::string version;

            // Callback function for command parsers.
            void (*callback)(std::string cmd_name, ArgParser& cmd_parser);

            // Register flags and options.
            void flag(std::string const& name);
            void option(std::string const& name, std::string const& fallback = "");

            // Parse the application's command line arguments.
            void parse(int argc, char **argv);
            void parse(std::vector<std::string> args);

            // Retrieve flag and option values.
            bool found(std::string const& name) const;
            int count(std::string const& name) const;
            std::string value(std::string const& name) const;
            std::vector<std::string> values(std::string const& name) const;

            template<typename T>
            T value(std::string const& name) const
            {
                std::string str = value(name);
                T ret = scan<T>(str);
                return ret;
            }

            template<typename T>
            std::vector<T> values(std::string const& name) const
            {
                std::vector<std::string> vec = values(name);
                std::vector<T> ret;
                ret.reserve(vec.size);
                for (size_t i = 0; i < vec.size(); ++i) {
                    T value = vec[i];
                    ret.push_back(value);
                }
                return ret;
            }

            // Register a command. Returns the command's ArgParser instance.
            ArgParser& command(
                std::string const& name,
                std::string const& helptext = "",
                void (*callback)(std::string cmd_name, ArgParser& cmd_parser) = nullptr
            );

            // Utilities for handling commands manually.
            bool commandFound();
            std::string commandName();
            ArgParser& commandParser();

            // Print a parser instance to stdout.
            void print();

        private:
            std::map<std::string, Option*> options;
            std::map<std::string, Flag*> flags;
            std::map<std::string, ArgParser*> commands;
            std::string command_name;

            void parse(ArgStream& args);
            void registerOption(std::string const& name, Option* option);
            void parseLongOption(std::string arg, ArgStream& stream);
            void parseShortOption(std::string arg, ArgStream& stream);
            void parseEqualsOption(std::string prefix, std::string name, std::string value);
            void exitHelp();
            void exitVersion();

            template<typename T>
            T scan(const std::string &input) const
            {
                std::stringstream stream(input);
                T ret;
                stream >> ret;
                return ret;
            }
    };
}

#endif
