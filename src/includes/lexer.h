#pragma once

#include <string>
#include <llparser.hpp> // Dependencia de la biblioteca DeSiGNAR

namespace Braille3D {
    class LexerB3D {
    public:
        // Constructor: Configura las reglas automáticamente al instanciar el objeto
        LexerB3D();

        // Método opcional expuesto para inicializar o reiniciar las reglas léxicas
        void setup_rules();

        // Tokeniza el código fuente y filtra espacios en blanco y comentarios
        Designar::DynArray<Designar::Token> tokenize(const std::string& source_code) const;
        
        // Devuelve todos los tokens sin filtrar (útil para depuración)
        Designar::DynArray<Designar::Token> tokenize_raw(const std::string& source_code) const;

    private:
        Designar::Lexer lexer_;
    };
} // namespace lexer