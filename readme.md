# Janus for C++

A C++ library for parsing command line arguments.


#### Features

* Long-form boolean flags with single-character shortcuts: `--flag`, `-f`.

* Long-form string, integer, and floating-point options with
  single-character shortcuts: `--option <arg>`, `-o <arg>`.

* Condensed short-form options: `-abc <arg> <arg>`.

* Automatic `--help` and `--version` flags.

* Support for multivalued options.

* Support for git-style command interfaces with arbitrarily-nested commands.


#### Links

* [Documentation][docs]
* [Sample Application][sample]


[docs]: https://darrenmulholland.com/docs/janus-cpp/
[sample]: https://github.com/dmulholland/janus-cpp/blob/master/src/example.cpp
