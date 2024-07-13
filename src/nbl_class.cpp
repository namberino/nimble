#include "nbl_class.hpp"

NblClass::NblClass(std::string name, std::map<std::string, std::shared_ptr<NblFunction>> methods)
    : name(std::move(name)), methods(std::move(methods)) {}

std::shared_ptr<NblFunction> NblClass::find_method(const std::string& name)
{
    auto element = methods.find(name);
    
    if (element != methods.end())
        return element->second;

    return nullptr;
}

int NblClass::arity()
{
    std::shared_ptr<NblFunction> initializer = find_method("init");
    
    if (initializer == nullptr)
        return 0;

    return initializer->arity();
}

std::any NblClass::call(Interpreter& interpreter, std::vector<std::any> arguments)
{
    auto instance = std::make_shared<NblInstance>(shared_from_this());
    std::shared_ptr<NblFunction> initializer = find_method("init");

    if (initializer != nullptr)
        initializer->bind(instance)->call(interpreter, std::move(arguments));

    return instance;
}

std::string NblClass::to_string()
{
    return name;
}
