#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include "scanner.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

void run(const std::string& text);
void run_file(const std::string& filename);
void run_prompt();

Interpreter interpreter{};

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
    std::vector<Token> tokens = scanner.scan_tokens();

    Parser parser{tokens};
    std::shared_ptr<Expr> expression = parser.parse();

    if (Error::has_error) // syntax error
        return;

    interpreter.interpret(expression);

    // std::cout << AstPrinter{}.print(expression) + "\n";
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
        exit(2);
    
    if (Error::has_runtime_error)
        exit(3);
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
