#include "ast_printer.hpp"

// testing code
int main(int argc, char* argv[])
{
    std::shared_ptr<Expr> expression = std::make_shared<BinaryExpr>(
        std::make_shared<UnaryExpr>(
            Token{TokenType::MINUS, "-", nullptr, 1},
            std::make_shared<LiteralExpr>(123.)
        ), // -123
        
        Token{TokenType::STAR, "*", nullptr, 1}, // multiply

        std::make_shared<GroupingExpr>(
            std::make_shared<LiteralExpr>(45.67)
        ) // 45.67
    );

    std::cout << AstPrinter{}.print(expression) << "\n";
}
