#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstring>

#include "nbl_callable.hpp"

class NativeClock : public NblCallable
{
    public:
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

class NativeTime : public NblCallable
{
    public:
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

class NativeRead : public NblCallable
{
    public:
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

class NativeExit : public NblCallable
{
    public:
        int param_count;
        
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> args) override;
        std::string to_string() override;
};

#endif
