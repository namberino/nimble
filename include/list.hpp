//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef LIST_HPP
#define LIST_HPP

#pragma once
#include <vector>
#include <any>

#include "expr.hpp"

struct ListType
{
    std::vector<std::any> elements;

    void append(std::any value);
    std::any get_element_at(int index);
    bool set_element_at(int index, std::any value);
    int get_length();
};

#endif
