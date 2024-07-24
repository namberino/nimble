//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <any>
#include <memory>
#include "filesystem"

#include "lexer.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "stmt.hpp"
#include "resolver.hpp"
#include "interpreter.hpp"

#define ANSI_RED "\033[0;31m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"

extern void run(const std::string& text, Interpreter& interpreter);
extern void run_file(const std::string& filename, Interpreter& interpreter);
extern void run_prompt(Interpreter& interpreter);

#endif
