//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef CLASS_HPP
#define CLASS_HPP

#pragma once
#include <any>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <map>

#include "callable.hpp"
#include "instance.hpp"
#include "function.hpp"

class Interpreter;
class NblFunction;

class NblClass : public NblCallable, public std::enable_shared_from_this<NblClass>
{
    friend class NblInstance;
    
    private:
        std::string name;
        std::shared_ptr<NblClass> superclass;
        std::map<std::string, std::shared_ptr<NblFunction>> methods;

    public:
        NblClass(std::string name, std::shared_ptr<NblClass> superclass, std::map<std::string, std::shared_ptr<NblFunction>> methods);
        std::shared_ptr<NblFunction> find_method(const std::string& name);
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
        std::string to_string() override;
};

#endif
