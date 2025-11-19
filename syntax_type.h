#ifndef SYNTAX_TYPE_H
#define SYNTAX_TYPE_H

// Enum to represent Common Lisp character syntax types.
enum SyntaxType {
    InvalidCharacter = 0,
    Constituent,
    MacroCharacter,
    SingleEscape,
    MultipleEscape,
    Whitespace
};

void init_lexical_scanner();

// Function to determine the syntax type of a given character.
SyntaxType syntax_type(char c);

#endif // SYNTAX_TYPE_H
