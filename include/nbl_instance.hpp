#ifndef NBL_INSTANCE_HPP
#define NBL_INSTANCE_HPP

#pragma once
#include <any>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "nbl_class.hpp"
#include "token.hpp"

class NblClass;
class Token;

class NblInstance : public std::enable_shared_from_this<NblInstance>
{
    private:
        std::shared_ptr<NblClass> klass;
        std::map<std::string, std::any> fields;

    public:
        NblInstance(std::shared_ptr<NblClass> klass);
        std::any get(const Token& name);
        void set(const Token& name, std::any value);
        std::string to_string();
};

#endif
