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
        std::cout << "Usage: nimble <script>.nbl\n";
        exit(1);
    }
    else if (argc == 2) // run script file
    {
        char* point = strrchr(argv[1], '.');

        if(point != NULL)
        {
            if(strcmp(point, ".nbl") != 0) // ends with nbl
            {
                std::cout << "Invalid file extension, please use '.nbl'\n";
                exit(1);
            }
        }
        else
        {
            std::cout << "Invalid file extension, please use '.nbl'\n";
            exit(1);
        }

        run_file(argv[1]);
    }
    else // run interactive mode
    {
        run_prompt();
    }

    return 0;
}

void run(const std::string& source)
{
    Scanner scanner{source};
    const auto tokens = scanner.scan_tokens();

    for (const auto& token : tokens)
        std::cout << token.to_string() + "\n";
}

void run_file(const std::string& path)
{
    std::cout << "Executing file: " + path + "\n";
    std::ifstream file{path};
    std::string line;
    std::string file_content;

    while (std::getline(file, line))
        file_content += line + "\n";

    run(file_content);
    
    if (Error::has_error)
    {
        exit(2);
    }
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
