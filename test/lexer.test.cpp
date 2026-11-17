/** @file lexer.test.cpp
    @brief Automated tests of the braille3d lexer (Braille3D::LexerB3D).

    The lexer turns source text into tokens, so every test follows the same
    shape: scan a small source string and compare the sequence of token
    names with the expected one. For example, scanning `PLATE {` must give
    `KW_PLATE LBRACE`.

    The tests are grouped by the rule they protect:

    - keywords and the unit `mm` are never read as identifiers;
    - numbers, identifiers and strings, including the Spanish characters
      that strings accept;
    - the operator and the delimiters;
    - whitespace and comments, which tokenize() drops and tokenize_raw()
      keeps;
    - the characters a comment accepts, which a string does not;
    - lexical errors, which must be reported as std::runtime_error;
    - the example programs in examples/, which must scan without errors.

    How it works. There is no test framework. Every check() prints one line
    with its result, [ OK ] or [FAIL], under the heading of its group, and
    the program ends with a summary: the checks passed per group, the total,
    and the list of the checks that failed. main() returns 0 when every
    check passed and 1 otherwise, which is what `make test` expects. A check
    that fails does not stop the program, so a single run lists every broken
    rule.

    How to add a test. Write a function that receives the lexer, start it
    with begin_group() and a short title, call check() with the condition and
    a short sentence that names the behavior, and call the function from
    main(). The sentence is what is printed next to the result, so it should
    say what must work.

    Regression note. The whitespace tests exist because the WS rule once
    matched the letters n, t and r instead of line breaks and tabs: a
    DeSiGNAR pattern gives no special meaning to the backslash sequences
    \n, \t and \r. The checks on line breaks, tabs and the letters n, t, r
    fail if that mistake comes back.
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <braille3d/lexer.hpp>

namespace
{
    /** Checks passed and total checks of one group of tests. */
    struct GroupResult
    {
        std::string title;
        int passed;
        int total;
    };

    /** The groups run so far, in order, for the final summary. */
    std::vector<GroupResult> groups;

    /** The failed checks, written as "<group>: <description>", for the final
        summary. */
    std::vector<std::string> failed_checks;

    /** Starts a group of tests: prints its heading and opens its counters.
        Call it first in every test function. */
    void begin_group(const std::string& title)
    {
        std::cout << "\n== " << title << "\n";
        groups.push_back(GroupResult{title, 0, 0});
    }

    /** Records one check of the current group and prints its result as
        [ OK ] or [FAIL] followed by `what`. A failed check does not stop the
        program, so that all the broken rules are reported in one run. */
    void check(bool condition, const std::string& what)
    {
        GroupResult& group = groups.back();

        ++group.total;
        if (condition)
        {
            ++group.passed;
            std::cout << "  [ OK ] " << what << "\n";
        }
        else
        {
            std::cout << "  [FAIL] " << what << "\n";
            failed_checks.push_back(group.title + ": " + what);
        }
    }

    /** Prints the summary and returns the number of failed checks: the
        checks passed in every group, the totals, and the list of the checks
        that failed. */
    int print_summary()
    {
        int passed = 0;
        int total = 0;

        std::cout << "\n== summary\n";
        for (const GroupResult& group : groups)
        {
            std::cout << "  " << std::left << std::setw(24) << group.title
                      << group.passed << "/" << group.total << "\n";
            passed += group.passed;
            total += group.total;
        }
        std::cout << "  " << std::left << std::setw(24) << "total" << passed
                  << "/" << total << "\n";

        if (!failed_checks.empty())
        {
            std::cout << "\nfailed checks:\n";
            for (const std::string& failed : failed_checks)
            {
                std::cout << "  - " << failed << "\n";
            }
        }
        return static_cast<int>(failed_checks.size());
    }

    /** Joins the names of `tokens` with one space, for example
        "KW_PLATE LBRACE". Comparing this text is how the tests describe the
        expected token sequence. */
    std::string names(const Designar::DynArray<Designar::Token>& tokens)
    {
        std::string result;
        for (Designar::nat_t i = 0; i < tokens.size(); ++i)
        {
            if (i > 0)
            {
                result += " ";
            }
            result += tokens[i].name;
        }
        return result;
    }

    /** Scans `source` with tokenize() and returns the token names, so
        whitespace and comments are not part of the result.
        If the lexer rejects the source, it returns "LEXICAL_ERROR: <reason>"
        instead of throwing: the check that called it then fails with a
        readable message, and the remaining checks still run. */
    std::string scan(const Braille3D::LexerB3D& lexer,
                     const std::string& source)
    {
        try
        {
            return names(lexer.tokenize(source));
        }
        catch (const std::runtime_error& error)
        {
            return std::string("LEXICAL_ERROR: ") + error.what();
        }
    }

    /** Same as scan(), but with tokenize_raw(): the result keeps the WS and
        COMMENT tokens. */
    std::string scan_raw(const Braille3D::LexerB3D& lexer,
                         const std::string& source)
    {
        try
        {
            return names(lexer.tokenize_raw(source));
        }
        catch (const std::runtime_error& error)
        {
            return std::string("LEXICAL_ERROR: ") + error.what();
        }
    }

    /** Returns true when the lexer rejects `source` with a
        std::runtime_error, which is how Designar::Lexer reports the first
        character that no rule can match. */
    bool is_lexical_error(const Braille3D::LexerB3D& lexer,
                          const std::string& source)
    {
        try
        {
            lexer.tokenize(source);
        }
        catch (const std::runtime_error&)
        {
            return true;
        }
        return false;
    }

    /** Returns the whole content of the file at `path`, or an empty string
        when it cannot be read. */
    std::string read_file(const std::string& path)
    {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /** Every reserved word is scanned as its own KW_ token. The token name
        is the word with the prefix KW_, so one loop covers all of them.
        Registering a keyword after the ID rule, or forgetting it, makes
        this test fail. */
    void test_keywords(const Braille3D::LexerB3D& lexer)
    {
        begin_group("keywords");

        const char* words[] = {
            "PLATE",    "BRAILLE",    "TEXT",        "USE",     "TEMPLATE",
            "WIDTH",    "HEIGHT",     "THICKNESS",   "MARGIN",  "RELIEF",
            "MATERIAL", "POSITION",   "ORIENTATION", "CONTENT", "PLA",
            "PETG",     "CENTER",     "LEFT",        "RIGHT",   "TOP",
            "BOTTOM",   "HORIZONTAL", "VERTICAL"};

        for (const char* word : words)
        {
            check(scan(lexer, word) == std::string("KW_") + word,
                  std::string("the word ") + word + " is a keyword");
        }
    }

    /** Integers and decimals are NUMBER, and the unit mm is UNIT_MM whether
        it is glued to the number or separated by a space. The glued case
        relies on the longest match: the lexer reads 50, then mm. */
    void test_numbers_and_unit(const Braille3D::LexerB3D& lexer)
    {
        begin_group("numbers and unit");

        check(scan(lexer, "50") == "NUMBER", "an integer is a NUMBER");
        check(scan(lexer, "0.6") == "NUMBER", "a decimal is a NUMBER");
        check(scan(lexer, "50mm") == "NUMBER UNIT_MM",
              "50mm is a number followed by the unit");
        check(scan(lexer, "0.6mm") == "NUMBER UNIT_MM",
              "0.6mm is a decimal followed by the unit");
        check(scan(lexer, "50 mm") == "NUMBER UNIT_MM",
              "a space between the number and mm is allowed");
    }

    /** Names chosen by the programmer are ID. A keyword must not turn the
        identifiers that merely start with it into keywords: PLATEX is an
        ID because the longest match wins. Keywords are case sensitive, so
        the lowercase plate is an ID, and mm is the unit and never an ID. */
    void test_identifiers(const Braille3D::LexerB3D& lexer)
    {
        begin_group("identifiers");

        check(scan(lexer, "room") == "ID", "a lowercase name is an ID");
        check(scan(lexer, "_x1") == "ID",
              "a name may start with an underscore and contain digits");
        check(scan(lexer, "PLATEX") == "ID",
              "a name that starts with a keyword is still one ID");
        check(scan(lexer, "plate") == "ID", "keywords are case sensitive");
        check(scan(lexer, "Content") == "ID",
              "only the uppercase CONTENT is the keyword");
        check(scan(lexer, "mm") == "UNIT_MM", "mm is the unit, not an ID");
    }

    /** Text between double quotes is one STRING token, also when it holds
        Spanish characters. The accented text is written with its UTF-8
        bytes (a-acute is 0xC3 0xA1, n-tilde is 0xC3 0xB1) so the test does
        not depend on the encoding of this source file. The last check
        compares the lexeme, which must keep every byte of the text. */
    void test_strings(const Braille3D::LexerB3D& lexer)
    {
        begin_group("strings");

        const std::string plain = "\"CENTRO DE SALUD\"";
        const std::string tilde = "\"Ni\xc3\xb1os y Ni\xc3\xb1"
                                  "as\"";
        const std::string vowels =
            "\"\xc3\xa1\xc3\xa9\xc3\xad\xc3\xb3\xc3\xba \xc3\x81\xc3\x89\"";

        check(scan(lexer, plain) == "STRING", "a plain string is a STRING");
        check(scan(lexer, tilde) == "STRING",
              "a string with n with tilde is a STRING");
        check(scan(lexer, vowels) == "STRING",
              "a string with accented vowels is a STRING");
        check(scan(lexer, "\"\"") == "STRING", "an empty string is a STRING");
        check(scan(lexer, tilde) == "STRING" &&
                  lexer.tokenize(tilde)[0].lexeme == tilde,
              "the lexeme keeps every byte of the string");
    }

    /** The assignment operator and every delimiter have their own token.
        The second check has no spaces, to show that the delimiters split
        the text by themselves. */
    void test_delimiters(const Braille3D::LexerB3D& lexer)
    {
        begin_group("delimiters");

        check(scan(lexer, "= { } ( ) , ;") ==
                  "ASSIGN LBRACE RBRACE LPAREN RPAREN COMMA SEMICOLON",
              "the operator and the delimiters have their own tokens");
        check(scan(lexer, "(1,2)") == "LPAREN NUMBER COMMA NUMBER RPAREN",
              "delimiters split the text without spaces");
    }

    /** Whitespace and comments separate the tokens but carry no meaning:
        tokenize() drops them and tokenize_raw() keeps them as WS and
        COMMENT. Line breaks, tabs and carriage returns must count as
        whitespace, and the letters n, t and r must not (see the regression
        note at the top of the file). */
    void test_whitespace_and_comments(const Braille3D::LexerB3D& lexer)
    {
        begin_group("whitespace and comments");

        check(scan(lexer, "PLATE\n{") == "KW_PLATE LBRACE",
              "a line break is whitespace");
        check(scan(lexer, "PLATE\t{") == "KW_PLATE LBRACE",
              "a tab is whitespace");
        check(scan(lexer, "PLATE\r\n{\r\n}") == "KW_PLATE LBRACE RBRACE",
              "a carriage return followed by a line break is whitespace");
        check(scan(lexer, "n t r") == "ID ID ID",
              "the letters n, t and r are identifiers, not whitespace");
        check(scan(lexer, "# a comment\nPLATE") == "KW_PLATE",
              "a comment on its own line is dropped");
        check(scan(lexer, "PLATE # trailing\n{") == "KW_PLATE LBRACE",
              "a comment after a token is dropped");

        check(scan_raw(lexer, "PLATE {") == "KW_PLATE WS LBRACE",
              "tokenize_raw keeps the whitespace");
        check(scan_raw(lexer, "# note\nPLATE") == "COMMENT WS KW_PLATE",
              "tokenize_raw keeps the comments and the line breaks");
    }

    /** A comment is free text that nobody translates to braille, so it
        accepts more characters than a string: the apostrophe, the symbols
        of everyday text, the Spanish opening question and exclamation marks
        and the accented letters. Only the double quote, the braces and the
        line break end it. A string keeps its closed list of characters, so
        the same symbols are still rejected inside quotes. */
    void test_comment_characters(const Braille3D::LexerB3D& lexer)
    {
        begin_group("comment characters");

        check(scan(lexer, "# don't\nPLATE") == "KW_PLATE",
              "a comment accepts the apostrophe");
        check(scan(lexer, "# $5 50% a&b @home\nPLATE") == "KW_PLATE",
              "a comment accepts $, %, & and @");
        check(scan(lexer, "# <a> [b] ^c ~d `e`\nPLATE") == "KW_PLATE",
              "a comment accepts <, >, [, ], ^, ~ and the backtick");
        check(scan(lexer, "# a\\b\nPLATE") == "KW_PLATE",
              "a comment accepts the backslash");
        check(scan(lexer, "# \xc2\xbfQu\xc3\xa9 hora es? "
                          "\xc2\xa1Listo!\nPLATE") == "KW_PLATE",
              "a comment accepts the Spanish question and exclamation marks");
        check(scan(lexer, "# Caf\xc3\xa9 de a\xc3\xb1o") == "",
              "a comment accepts Spanish letters and may end the file");
        check(scan(lexer, "# a \"q\" b") == "STRING ID",
              "a double quote ends a comment");
        check(scan(lexer, "# x { y") == "LBRACE ID", "a brace ends a comment");

        check(is_lexical_error(lexer, "\"it's\""),
              "a string still rejects the apostrophe");
        check(is_lexical_error(lexer, "\"50%\""),
              "a string still rejects the percent sign");
        check(is_lexical_error(lexer, "\"\xc2\xbfQu\xc3\xa9?\""),
              "a string still rejects the opening question mark");
    }

    /** A character that no rule accepts is a lexical error. The tests use
        a stray character, an unterminated string, and a number that ends
        with a dot. */
    void test_errors(const Braille3D::LexerB3D& lexer)
    {
        begin_group("lexical errors");

        check(is_lexical_error(lexer, "@"), "a stray character is an error");
        check(is_lexical_error(lexer, "PLATE @"),
              "a stray character after a valid token is an error");
        check(is_lexical_error(lexer, "\"unterminated"),
              "a string without the closing quote is an error");
        check(is_lexical_error(lexer, "5."),
              "a number that ends with a dot is an error");
    }

    /** The example programs of examples/ are valid braille3d, so each one
        must scan without a lexical error and produce tokens. The paths are
        relative to the project root, which is where `make test` runs. */
    void test_examples(const Braille3D::LexerB3D& lexer)
    {
        begin_group("example programs");

        const char* files[] = {"examples/health-center.b3d",
                               "examples/room-template.b3d",
                               "examples/accessible-sign.b3d"};

        for (const char* path : files)
        {
            const std::string source = read_file(path);

            check(!source.empty(),
                  std::string("the example ") + path + " can be read");
            check(!is_lexical_error(lexer, source),
                  std::string("the example ") + path +
                      " scans without a lexical error");
            check(scan(lexer, source) != "",
                  std::string("the example ") + path + " produces tokens");
        }
    }
} // namespace

/** Runs every group of tests with one lexer, prints the summary and returns 0
    when all checks passed, or 1 otherwise. */
int main()
{
    const Braille3D::LexerB3D lexer;

    test_keywords(lexer);
    test_numbers_and_unit(lexer);
    test_identifiers(lexer);
    test_strings(lexer);
    test_delimiters(lexer);
    test_whitespace_and_comments(lexer);
    test_comment_characters(lexer);
    test_errors(lexer);
    test_examples(lexer);

    return print_summary() > 0 ? 1 : 0;
}
