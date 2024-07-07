#include "error.hpp"

bool Error::has_error = false;

void Error::report(int line, const std::string& where, const std::string& msg)
{
    std::cout << "On line " + std::to_string(line) + ". Error " + where + ": " + msg + "\n";
}

void Error::error(int line, const std::string& msg)
{
    report(line, "", msg);
    has_error = true;
}
