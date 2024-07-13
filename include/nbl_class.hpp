#ifndef NBL_CLASS_HPP
#define NBL_CLASS_HPP

#pragma once
#include <any>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <map>

#include "nbl_callable.hpp"
#include "nbl_instance.hpp"
#include "nbl_function.hpp"

class Interpreter;
class NblFunction;

class NblClass : public NblCallable, public std::enable_shared_from_this<NblClass>
{
    friend class NblInstance;
    
    private:
        std::string name;
        std::map<std::string, std::shared_ptr<NblFunction>> methods;

    public:
        NblClass(std::string name, std::map<std::string, std::shared_ptr<NblFunction>> methods);
        std::shared_ptr<NblFunction> find_method(const std::string& name);
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
        std::string to_string() override;
};

#endif
