#include "builtins.hpp"

int NativeClock::arity()
{
    return 0;
}

std::any NativeClock::call(Interpreter& interpreter, std::vector<std::any> args)
{
    auto ticks = std::chrono::system_clock::now().time_since_epoch();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(ticks).count());
}

std::string NativeClock::to_string()
{
    return "<native clock>";
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
