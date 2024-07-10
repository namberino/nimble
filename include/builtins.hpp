#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <chrono>

#include "nbl_callable.hpp"

class NativeClock : public NblCallable
{
    public:
        int arity() override
        {
            return 0;
        }

        std::any call(Interpreter& interpreter, std::vector<std::any> args) override
        {
            auto ticks = std::chrono::system_clock::now().time_since_epoch();
            return std::chrono::duration<double>{ticks}.count() / 1000.0;
        }

        std::string to_string() override
        {
            return "<native clock>";
        }
};

#endif
