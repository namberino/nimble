#include "error.hpp"

bool Error::has_error = false;

void Error::report(int line, const std::string& where, const std::string& msg)
{
    std::cout << "Line: " + std::to_string(line) + ", Error " + where + ": " + msg + "\n";
    has_error = true;
}

void Error::error(int line, const std::string& msg)
{
    report(line, "", msg);
}
