/** @file main.cpp
    @brief Command-line driver: scans a braille3d source file and prints its
    tokens. Pass the file as the first argument.
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <braille3d/lexer.hpp>

namespace
{
    /** Returns the whole content of the file at `filepath`.
        Throws std::runtime_error if the file cannot be opened. */
    std::string read_file(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open source file: " + filepath);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
} // namespace

int main(int argc, char* argv[])
{
    // Without an argument, scan the default example.
    const std::string filepath =
        (argc > 1) ? argv[1] : "examples/health-center.b3d";

    try
    {
        const std::string code = read_file(filepath);

        // The constructor registers every token rule.
        const Braille3D::LexerB3D lexer;

        std::cout << "=== [Lexer] Scanning file: " << filepath << " ===\n\n";

        const Designar::DynArray<Designar::Token> tokens = lexer.tokenize(code);

        std::cout << "Significant tokens found: " << tokens.size() << "\n";
        std::cout << "--------------------------------------------------\n";

        for (Designar::nat_t i = 0; i < tokens.size(); ++i)
        {
            std::cout << "[" << i << "] TOKEN: " << tokens[i].name
                      << "\t | LEXEME: " << tokens[i].lexeme << "\n";
        }

        std::cout << "--------------------------------------------------\n";
        std::cout << "[OK] No lexical errors. The token stream is valid for "
                     "the parser.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Lexer error]: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
