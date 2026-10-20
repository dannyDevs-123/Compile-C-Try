#include "parser.hpp"
#include "scanner.hpp"

#include <exception>
#include <iostream>
#include <string>

int main() {
    const std::string source = R"(
        // Arithmetic respects the usual operator precedence.
        let base = 12;
        let factor = 2.5;
        total = (base + 4) * factor / 2;
        print total; /* End of the sample program. */
    )";

    try {
        Scanner scanner(source);
        const std::vector<Token> tokens = scanner.scanTokens();
        Parser parser(tokens);
        const Program program = parser.parse();
        std::cout << "AST:\n";
        printAst(std::cout, program);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Compilation failed: " << error.what() << '\n';
        return 1;
    }
}
