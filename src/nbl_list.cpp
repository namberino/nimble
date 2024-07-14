#include "nbl_list.hpp"

void ListType::append(std::any value)
{
    elements.push_back(value);
}

std::any ListType::get_element_at(int index)
{
    return elements.at(index);
}
