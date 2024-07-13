#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#pragma once
#include <memory>
#include <any>
#include <string>
#include <map>
#include <functional>
#include <utility>

#include "error.hpp"
#include "token.hpp"

class Environment : public std::enable_shared_from_this<Environment>
{
    friend class Interpreter;

    std::shared_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;

    public:
        Environment();
        Environment(std::shared_ptr<Environment> enclosing);

        std::any get(const Token& name);
        void assign(const Token& name, std::any value);
        void define(const std::string& name, std::any value);
        std::shared_ptr<Environment> ancestor(int distance);
        std::any get_at(int distance, const std::string& name);
        void assign_at(int distance, const Token& name, std::any value);
};

#endif
