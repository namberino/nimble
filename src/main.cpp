#include <iostream>
#include <fstream>
#include <string>

#include "scanner.hpp"
#include "error.hpp"

void run(const std::string& text);
void run_file(const std::string& filename);
void run_prompt();

int main(int argc, char* argv[])
{
    if (argc > 2) // too many arguments
    {
        std::cout << "Usage: nimble <script>.nb\n";
        exit(1);
    }
    else if (argc == 2) // run script file
    {
        run_file(argv[1]);
    }
    else // run interactive mode
    {
        run_prompt();
    }

    return 0;
}

void run(const std::string& text)
{
    Scanner scanner{text};
    const auto tokens = scanner.scan_tokens();

    for (const auto& token : tokens)
        std::cout << token.to_string() + "\n";
}

void run_file(const std::string& path)
{
    std::cout << "Executing file: " + path + "\n";
    std::ifstream file{path};
    std::string line;
    std::string text;

    while (std::getline(file, line))
        text += line + "\n";

    run(text);
    
    if (Error::has_error)
        exit(2);
}

void run_prompt()
{
    std::string text;

    while (true)
    {
        std::cout << "nimble% ";

        if (std::getline(std::cin, text))
        {
            run(text);
            Error::has_error = false;
        }
        else
        {
            break;
        }
    }
}
