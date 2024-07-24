#include "util.hpp"

void run(const std::string& source, Interpreter& interpreter, std::string base_dir)
{
    Lexer lexer{source};
    std::vector<Token> tokens = lexer.scan_tokens();

    Parser parser{tokens};
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (Error::has_error) // syntax error
        return;

    Resolver resolver{interpreter, base_dir};
    resolver.resolve(statements);

    if (Error::has_error) // resolution error
        return;

    interpreter.interpret(statements);

    // std::cout << AstPrinter{}.print(expression) + "\n";
}

void run_file(const std::string& path, Interpreter& interpreter)
{
    // std::cout << "Executing file: " + path + "\n";
    std::ifstream file{path};
    std::string line;
    std::string file_content;

    while (std::getline(file, line))
        file_content += line + "\n";

    run(file_content, interpreter, path);
    
    if (Error::has_error)
        exit(2);
    
    if (Error::has_runtime_error)
        exit(3);
}

void run_prompt(Interpreter& interpreter)
{
    std::string text;
    std::string base_dir = fs::current_path().string();
    
    while (true)
    {
        std::cout << ANSI_CYAN << "nimble" << ANSI_RED << "% " << ANSI_RESET;

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
            
            Resolver resolver{interpreter, base_dir};
            resolver.resolve(std::any_cast<std::vector<std::shared_ptr<Stmt>>>(syntax));

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
