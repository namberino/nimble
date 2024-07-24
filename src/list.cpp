//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#include "list.hpp"

void ListType::append(std::any value)
{
    elements.push_back(value);
}

std::any ListType::get_element_at(int index)
{
    return elements.at(index);
}

int ListType::get_length()
{
    return elements.size();
}

bool ListType::set_element_at(int index, std::any value)
{
    if (index == get_length())
        elements.insert(elements.begin() + index, value);
    else if (index < get_length() && index >= 0)
        elements[index] = value;
    else
        return false;
    
    return true;
}
