#include <braille3d/lexer.hpp>
namespace Braille3D 
{
    LexerB3D::LexerB3D() 
    {
        setup_rules();
    }
    
    void LexerB3D::setup_rules()
    {
        
        const std::string DIGIT = "(0|1|2|3|4|5|6|7|8|9)";
        const std::string LOWER = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)";
        const std::string UPPER = "(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)";
        const std::string MIX = "(" + LOWER + "|" + UPPER + "|_)";
        const std::string ALNUM = "(" + MIX + "|" + DIGIT + ")";
        
        
        // Palabras reservadas y Unidad de medida
        lexer_.add_token("KW_PLATE",    "PLATE");
        lexer_.add_token("KW_BRAILLE",  "BRAILLE");
        lexer_.add_token("KW_TEXT",     "TEXT");
        lexer_.add_token("KW_USE",      "USE");
        lexer_.add_token("KW_TEMPLATE", "TEMPLATE");
        lexer_.add_token("KW_WIDTH",    "WIDTH");
        lexer_.add_token("KW_HEIGHT",   "HEIGHT");
        lexer_.add_token("KW_THICKNESS","THICKNESS");
        lexer_.add_token("KW_MARGIN",   "MARGIN");
        lexer_.add_token("KW_RELIEF",   "RELIEF");
        lexer_.add_token("KW_MATERIAL", "MATERIAL");
        lexer_.add_token("KW_POSITION", "POSITION");
        lexer_.add_token("KW_ORIENTATION","ORIENTATION");
        lexer_.add_token("KW_PLA",       "PLA");
        lexer_.add_token("KW_PETG",      "PETG");
        lexer_.add_token("KW_CENTER",    "CENTER");
        lexer_.add_token("KW_LEFT",      "LEFT");
        lexer_.add_token("KW_RIGHT",     "RIGHT");
        lexer_.add_token("KW_TOP",       "TOP");
        lexer_.add_token("KW_BOTTOM",    "BOTTOM");
        lexer_.add_token("KW_HORIZONTAL","HORIZONTAL");
        lexer_.add_token("KW_VERTICAL",  "VERTICAL");

        
        lexer_.add_token("UNIT_MM",  "mm");

        // Reglas léxicas y identificadores 
        lexer_.add_token("ID",MIX + ALNUM + "*");

        std::string number_regex = DIGIT + DIGIT + "*(\\." + DIGIT + DIGIT + "*)?";
        lexer_.add_token("NUMBER",  number_regex);
        
        std::string str_char = "(" + ALNUM + "| |\\t|\\-|_|\\.|,|:|;|!|\\?|\\(|\\)|/|\\+|\\*|=|#|\\||á|é|í|ó|ú|Á|É|Í|Ó|Ú|ñ|Ñ|ü|Ü)";        lexer_.add_token("STRING", "\"" + str_char + "*\"");
        
        // Operador y delimitadores
        lexer_.add_token("ASSIGN",  "=");
        lexer_.add_token("LBRACE",  "\\{");
        lexer_.add_token("RBRACE",  "\\}");
        lexer_.add_token("LPAREN",  "\\(");
        lexer_.add_token("RPAREN",  "\\)");
        lexer_.add_token("COMMA",   ",");
        lexer_.add_token("SEMICOLON", ";");
        
        // Ignorados
        // Reglas de espacios y comentarios compatibles con el motor de DeSiGNAR
        // Usamos el espacio literal y la barra vertical para evitar conflictos de autómata
        std::string comment_char = "(" + str_char + "| )";
        lexer_.add_token("COMMENT",  "#" + comment_char + "*");
        lexer_.add_token("WS", "( |\t|\n|\r)( |\t|\n|\r)*");
    }

    //Devolvemos absolutamente todos los tokens del lenguaje 
    Designar::DynArray<Designar::Token> LexerB3D::tokenize_raw(const std::string& source_code) const{
        return lexer_.tokenize(source_code);
    }

    //Eliminamos la "basura" visual (espacios y comentarios) para que el árbol sintáctico
    Designar::DynArray<Designar::Token> LexerB3D::tokenize(const std::string& source_code) const {
        Designar::DynArray<Designar::Token> all_tokens = lexer_.tokenize(source_code);
        Designar::DynArray<Designar::Token> clean_tokens;
        for (Designar::nat_t i = 0; i < all_tokens.size(); ++i)
        {
            // Filtramos tanto los espacios (WS), como los comentarios (COMMENT)
            if (all_tokens[i].name != "WS" && all_tokens[i].name != "COMMENT") 
            {
                clean_tokens.append(all_tokens[i]);
            }
        }
        return clean_tokens;
    }
}// namespace lexer
