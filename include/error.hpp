#ifndef ERROR_HPP
#define ERROR_HPP

#pragma once
#include <string>
#include <iostream>
#include <stdexcept>

#include "token.hpp"

class RuntimeError : public std::runtime_error
{
    public:
        const Token& token;

        RuntimeError(const Token& token, std::string msg);
};

class Error
{
    public:
        static bool has_error; // general error
        static bool has_runtime_error; // runtime error

        static void report(int line, const std::string& where, const std::string& msg);
        static void error(int line, const std::string& msg);
        static void error(const Token& token, std::string msg);
        static void runtime_error(const RuntimeError& error);
};

#endif
