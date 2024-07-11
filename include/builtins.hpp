#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <chrono>
#include <iostream>
#include <sstream>

#include "nbl_callable.hpp"

class NativeClock : public NblCallable
{
    public:
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

class Read : public NblCallable
{
    public:
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

#endif
