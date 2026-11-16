#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <braille3d/lexer.hpp>

std::string read_file(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo fuente: " +
                                 filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[])
{
    // Si no se pasa un archivo por argumento
    std::string filepath = (argc > 1) ? argv[1] : "examples/centro-salud.b3d";

    try
    {
        std::string code = read_file(filepath);

        // Instanciar el Lexer (las reglas se configuran automáticamente en su
        // constructor)
        Braille3D::LexerB3D lexer;

        std::cout << "=== [Lexer3D] Analizando archivo: " << filepath
                  << " ===\n\n";

        // Ejecutar el escaneo de tokens mediante DeSiGNAR
        Designar::DynArray<Designar::Token> tokens = lexer.tokenize(code);

        std::cout << "Total de tokens significativos encontrados: "
                  << tokens.size() << "\n";
        std::cout << "--------------------------------------------------\n";

        for (Designar::nat_t i = 0; i < tokens.size(); ++i)
        {
            std::cout << "[" << i << "] TOKEN: " << tokens[i].name
                      << "\t | LEXEMA: " << tokens[i].lexeme << "\n";
        }

        std::cout << "--------------------------------------------------\n";
        std::cout << "[Exito] El archivo no posee errores lexicos. Flujo "
                     "valido para el Parser.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Error Lexer / Sistema]: " << e.what() << "\n";
        return 1;
    }

    return 0;
}