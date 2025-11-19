#include <cctype>
#include "syntax_type.h"

static SyntaxType syntax_table[128] = { SyntaxType::InvalidCharacter };

static void build_syntax_table()
{
    for (int i = 0; i < 128; ++i) {
        syntax_table[i] = SyntaxType::InvalidCharacter;
    }

    // Whitespace characters (as defined by isspace)
    for (int i = 0; i < 128; ++i) {
        if (std::isspace(i)) {
            syntax_table[i] = SyntaxType::Whitespace;
        }
    }
    
    // Alphanumeric characters are constituents
    for (int i = 0; i < 128; ++i) {
        if (std::isalnum(i)) {
            syntax_table[i] = SyntaxType::Constituent;
        }
    }

    // Other standard constituent characters
    // Note: '.' is complicated. It's a constituent but can be part of a number.
    // This simplified model treats it as a constituent.
    const char* other_constituents = "+-*/=<>&!?%$_{}[]~^.";
    for (const char* p = other_constituents; *p; ++p) {
        syntax_table[(unsigned char) *p] = SyntaxType::Constituent;
    }

    // Macro characters (these override any previous classification)
    syntax_table[('"')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)('\'')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)('`')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)(',')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)(';')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)('(')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)(')')] = SyntaxType::MacroCharacter;
    syntax_table[(unsigned char)('#')] = SyntaxType::MacroCharacter;

    // Escape characters
    syntax_table[(unsigned char)('\\')] = SyntaxType::SingleEscape;
    syntax_table[(unsigned char)('|')] = SyntaxType::MultipleEscape;
}

void init_lexical_scanner()
{
    build_syntax_table();
}

SyntaxType syntax_type(char c) {
    if (c >= 0 && c < 128) {
        return syntax_table[(unsigned char)(c)];
    } else {
        return SyntaxType::InvalidCharacter;
    }
}
