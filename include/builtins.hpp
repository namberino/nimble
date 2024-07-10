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
            auto now = std::chrono::system_clock::now().time_since_epoch();
            return static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(now).count());
        }

        std::string to_string() override
        {
            return "Native clock";
        }
};

#endif
