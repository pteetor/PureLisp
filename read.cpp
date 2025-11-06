//
//  read() function and supporting functions
//

#include "consVM.h"

static const char* PROMPT = ">> ";
static const char* CONTINUATION_PROMPT = ">>>> ";

enum Token {
  LPAREN,
  RPAREN,
  SYMBOL,
  START_OF_FILE,
  END_OF_FILE,
  UNDEF_TOKEN
};

// The current token
static const int TOKEN_SIZE = 256;
static char token_text[TOKEN_SIZE] = { '\0' };
static Token token = START_OF_FILE;

// The current input text
static const int BUFFER_SIZE = 1024;
static char buffer[BUFFER_SIZE] = { '\0' };
static int lineCount = 0;

// This points to the character immediately following the current token
static char* bufp;

// Local functions
static void init_reader();
static Cell* parse_sexpr();
static Cell* read_list();
static Token next_token();
static Token scan_symbol();
static char* find_token_start();
static bool fill_buffer();

static void init_reader()
{
  buffer[0] = '\0';
  bufp = &buffer[0];
  lineCount = 0;
}

// Here, 'token' is at the token before the sexpr.
// Leave 'token' at the last token of the sexpr.
Cell* read(bool top_level)
{
  if (top_level) {
    init_reader();
  }

  next_token();
  if (token == END_OF_FILE) {
    return NULL;
  } else {
    return parse_sexpr();
  }
}

// Here, 'token' is at the first token of the sexpr.
// Leave 'token' at the last token of the sexpr.
static Cell* parse_sexpr()
{
  switch (token)
  {
  case LPAREN:
    return read_list();
  case RPAREN:
    fatal("Extraneous right paren");   // BETTER: Throw error
  case SYMBOL:
    return atom(token_text);
  case END_OF_FILE:
    fatal("read: Premature end of file");
    return NULL;
  default:
    fatal("read: unhandled token type");
  }

  // Unreachable!
  return NULL;
}

static Cell* read_list()
{
  next_token();
  if (token == RPAREN) {
    return (Cell*) nil;
  }

  Cons* head = cons(parse_sexpr(), nil);
  Cons* tail = head;

  while (next_token() != RPAREN) {
    Cons* new_tail = cons(parse_sexpr(), nil);
    tail->cdr = (Cell *) new_tail;
    tail = new_tail;
  }

  return (Cell*) head;
}

static Token next_token()
{
  if (find_token_start() == NULL) {
    return (token = END_OF_FILE);
  }

  char ch = *bufp;

  switch (ch)
  {
  case '(':
    bufp++;
    return (token = LPAREN);
  case ')':
    bufp++;
    return (token = RPAREN);
  }

  if (std::isalnum(ch)) {
    return scan_symbol();
  }

  fatal("read: Non-alpha-numeric character");

  // unreachable!
  return UNDEF_TOKEN;
}

static Token scan_symbol()
{
  char* p = token_text;

  while(std::isalnum(*bufp)) {
    *p++ = *bufp++;
  }
  *p++ = '\0';

  return (token = SYMBOL);
}

static char* find_token_start()
{
  while (1) {
    while (*bufp && *bufp != ';') {   // ';' starts a comment
      if (!std::isspace(*bufp)) {
        return bufp;
      }
      bufp++;
    }

    if (!fill_buffer()) {
      return NULL;
    }
  }
}

static bool fill_buffer()
{
  buffer[0] = '\0';
  bufp = buffer;

  while (!std::cin.eof()) {
    std::cout << (lineCount == 0 ? PROMPT : CONTINUATION_PROMPT);
    std::cin.getline(buffer, BUFFER_SIZE);
    if (std::cin.fail()) {
      break;
    }
    ++lineCount;

    // Empty line?
    if (buffer[0] != '\n') {
      return true;
    }
  }

  std::cout << std::endl << "End of file" << std::endl;   // DEBUG
  return false;
}
