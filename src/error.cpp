//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#include "error.hpp"

bool Error::has_error = false;
bool Error::has_runtime_error = false;

void Error::report(int line, const std::string& where, const std::string& msg)
{
    // reporting error
    std::cout << "On line: " + std::to_string(line) + ", Error" + where + ": " + msg + "\n";
    has_error = true;
}

void Error::error(int line, const std::string& msg)
{
    // handling the error
    report(line, "", msg);
}

void Error::error(const Token& token, std::string msg)
{
    // parse error
    if (token.type == TOKEN_EOF)
        report(token.line, " at end", msg);
    else
        report(token.line, " at '" + token.lexeme + "'", msg);
}

RuntimeError::RuntimeError(const Token& token, std::string msg)
    : std::runtime_error(msg.data()), token(token) {}

void Error::runtime_error(const RuntimeError& error)
{
    // runtime error handling
    std::cout << error.what() << "\nOn line " << error.token.line << "\n";
    has_runtime_error = true;
}
