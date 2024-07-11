#include "builtins.hpp"

int NativeClock::arity()
{
    return 0;
}

std::any NativeClock::call(Interpreter& interpreter, std::vector<std::any> args)
{
    auto ticks = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration<double>{ticks}.count() / 100.0;
}

std::string NativeClock::to_string()
{
    return "<native clock>";
}


int NativeTime::arity()
{
    return 0;
}

std::any NativeTime::call(Interpreter& interpreter, std::vector<std::any> args)
{
    std::time_t current_time = std::time(nullptr);

    return std::string(std::ctime(&current_time));
}

std::string NativeTime::to_string()
{
    return "<native time>";
}


int Read::arity()
{
    return 1;
}

std::any Read::call(Interpreter& interpreter, std::vector<std::any> args)
{
    std::string prompt = std::any_cast<std::string>(args[0]);
    std::cout << prompt;

    std::string input;
    std::getline(std::cin, input);

    // stream the string into different variables
    std::istringstream iss(input);
    double result;

    if (!(iss >> result)) // input is not a double
        return input;

    return result;
}

std::string Read::to_string()
{
    return "<native read>";
}
