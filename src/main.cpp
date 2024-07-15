#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <any>
#include <memory>

#include "lexer.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include "resolver.hpp"

#define ANSI_RED "\033[0;31m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"

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
            if(strcmp(point, ".nbl") != 0 && strcmp(point, ".nimble") != 0) // ends with nbl
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
    Lexer lexer{source};
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser{tokens};
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (Error::has_error) // syntax error
        return;

    Resolver resolver{interpreter};
    resolver.resolve(statements);

    if (Error::has_error) // resolution error
        return;

    interpreter.interpret(statements);

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
        std::cout << ANSI_CYAN << "nimble" << ANSI_RED << "> " << ANSI_RESET;

        if (std::getline(std::cin, text))
        {
            Error::has_error = false;
            
            // run(text);
            Lexer lexer{text};
            std::vector<Token> tokens = lexer.scan_tokens();

            Parser parser{tokens};
            std::any syntax = parser.parse_repl();

            if (Error::has_error) // syntax error
            {
                std::cout << "Invalid syntax error\n";
                continue;
            }

            if (syntax.type() == typeid(std::vector<std::shared_ptr<Stmt>>))
            {
                interpreter.interpret(std::any_cast<std::vector<std::shared_ptr<Stmt>>>(syntax));
            }
            else if (syntax.type() == typeid(std::shared_ptr<Expr>))
            {
                std::string result = interpreter.interpret(std::any_cast<std::shared_ptr<Expr>>(syntax));

                if (result != "")
                    std::cout << result + "\n";
            }
        }
        else
        {
            break;
        }
    }
}
