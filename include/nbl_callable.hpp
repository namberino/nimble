#ifndef NBL_CALLABLE_HPP
#define NBL_CALLABLE_HPP

#pragma once
#include <vector>
#include <any>
#include <string>

class Interpreter;

class NblCallable
{
    public:
        virtual ~NblCallable() = default;
        virtual int arity() = 0;
        virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0;
        virtual std::string to_string() = 0;
};

#endif
