#include "nbl_list.hpp"

void ListType::append(std::any value)
{
    elements.push_back(value);
}
