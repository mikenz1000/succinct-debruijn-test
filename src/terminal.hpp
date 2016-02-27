/*
    Utility functions for dealing with the terminal 
*/
#ifndef __TERMINAL_HPP
#define __TERMINAL_HPP
#include <iostream>
#include <string>

namespace terminal {
    
// colour codes for terminal output
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string light_gray("\033[0;37m");
const std::string dark_gray("\033[0;30m");
const std::string reset("\033[0m");

};

#endif