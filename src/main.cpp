#include <iostream>

#include "util.hpp"

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

        run_file(argv[1], interpreter);
    }
    else // run interactive mode
    {
        run_prompt(interpreter);
        // prompt_load("./example/function/function-8.nbl");
    }

    return 0;
}
