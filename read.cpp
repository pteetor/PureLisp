//
//  read() function and supporting functions
//

#include "consVM.h"

static const char* PROMPT = ">> ";
static const char* CONTINUATION_PROMPT = ">>>> ";

enum Token {
  LPAREN,
  RPAREN,
  PERIOD,
  QUOTE,
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
static bool parse_sexpr();
static bool read_list();
static bool read_quote();

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
bool read(bool top_level)
{
  if (top_level) {
    init_reader();
  }

  next_token();
  if (token == END_OF_FILE) {
    return false;
  } else {
    return parse_sexpr();
  }
}

// Here, 'token' is at the first token of the sexpr.
// Leave 'token' at the last token of the sexpr.
static bool parse_sexpr()
{
  switch (token)
  {
  case LPAREN:
    return read_list();
  case RPAREN:
    throw LispError("Extraneous right paren");
  case PERIOD:
    throw LispError("Extraneous period");
  case QUOTE:
    return read_quote();
  case SYMBOL:
    push(atom(token_text));
    return true;
  case END_OF_FILE:
    throw LispError("read: Premature end of file");
  default:
    throw LispError("read: unhandled token type");
  }

  // Unreachable!
  return false;
}

static bool read_list()
{
  next_token();
  if (token == RPAREN) {
    push(nil);
    return true;
  }

  int nElem = 0;

  parse_sexpr();
  nElem++;

  for (;;) {
    next_token();
    if (token == RPAREN) {
        push(nil);
        break;
    }
    if (token == PERIOD) {
        next_token();
        parse_sexpr();
        if (next_token() != RPAREN) {
            LispError("Missing right paren after dot notation");
        }
        break;
    }
    parse_sexpr();
    nElem++;
  }

  while (nElem > 0) {
    cons();
    nElem--;
  }

  return true;
}

static bool read_quote()
{
    next_token();
    push(a_quote);
    parse_sexpr();
    push(nil);
    cons();
    cons();
    return true;
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
  case '.':
    bufp++;
    return (token = PERIOD);
  case '\'':
    bufp++;
    return (token = QUOTE);
  }

  if (std::isalnum(ch)) {
    return scan_symbol();
  }

  throw LispError("read: Non-alpha-numeric character");
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
