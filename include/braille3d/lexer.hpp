/** @file braille3d/lexer.hpp
    @brief Lexical analyzer for the braille3d language, built on the
    Designar::Lexer engine.

    LexerB3D registers every token rule of the language (keywords, the
    millimetre unit, identifiers, numbers, strings, delimiters, whitespace
    and comments) and exposes two scanning modes: tokenize(), which drops
    whitespace and comments so the output can feed a parser, and
    tokenize_raw(), which keeps every token for debugging.
*/

#pragma once

#include <string>

#include <array.hpp>
#include <lexer.hpp>

namespace Braille3D
{
    /** Scans braille3d source text into a sequence of Designar tokens.

        The rules are registered once, in the constructor. Keywords are
        registered before the identifier rule: Designar::Lexer breaks ties
        between equally long matches in favor of the rule registered first,
        so `PLATE` is a KW_PLATE token and not an ID. */
    class LexerB3D
    {
        Designar::Lexer lexer_;

    public:
        /** Builds the lexer and registers every rule of the language. */
        LexerB3D();

        /** Registers the token rules of the language. Called by the
            constructor; calling it again adds the same rules a second time,
            which is harmless but redundant. */
        void setup_rules();

        /** Scans `source_code` and returns its tokens without the WS and
            COMMENT tokens, ready to be consumed by a parser.
            Throws std::runtime_error at the first position no rule can
            match, which is a lexical error. */
        Designar::DynArray<Designar::Token>
        tokenize(const std::string& source_code) const;

        /** Scans `source_code` and returns every token, including WS and
            COMMENT. Useful to check that each character of the source is
            recognized by some rule. */
        Designar::DynArray<Designar::Token>
        tokenize_raw(const std::string& source_code) const;
    };
} // namespace Braille3D
