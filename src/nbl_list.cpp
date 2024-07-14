#include "nbl_list.hpp"

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

void ListType::set_element_at(int index, std::any value)
{
    elements.insert(elements.begin() + index, value);
}
