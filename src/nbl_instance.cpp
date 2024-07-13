#include "nbl_instance.hpp"

NblInstance::NblInstance(std::shared_ptr<NblClass> klass)
    : klass(std::move(klass)) {}

std::any NblInstance::get(const Token& name)
{
    auto element = fields.find(name.lexeme);

    if (element != fields.end())
        return element->second;

    std::shared_ptr<NblFunction> method = klass->find_method(name.lexeme);

    if (method != nullptr)
        return method->bind(shared_from_this());

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'");
}

void NblInstance::set(const Token& name, std::any value)
{
    fields[name.lexeme] = std::move(value);
}

std::string NblInstance::to_string()
{
    return klass->name + " instance";
}
