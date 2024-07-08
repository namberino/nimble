#ifndef ERROR_HPP
#define ERROR_HPP

#pragma once
#include <string>
#include <iostream>

#include "token.hpp"

class Error
{
    public:
        static bool has_error;

        static void report(int line, const std::string& where, const std::string& msg);
        static void error(int line, const std::string& msg);
        static void error(const Token& token, std::string msg);
};

#endif
