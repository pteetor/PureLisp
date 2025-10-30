//
//  read() function and supporting functions
//

#include "consVM.h"

// The current input text
static const int BUFFER_SIZE = 1024;
static char buffer[BUFFER_SIZE] = { '\0' };
static char* bufp;

enum Token {
  LPAREN,
  RPAREN,
  SYMBOL,
  START_OF_FILE,
  START_OF_LINE,
  END_OF_LINE,
  END_OF_FILE
};

// The current token
static const int TOKEN_SIZE = 256;
static char token_text[TOKEN_SIZE];
static Token token;

// Local functions
static Cell* read_list();
static Token next_token();
static char* find_token();
static bool fill_buffer();

void init_reader()
{
  buffer[0] = '\0';
  bufp = &buffer[0];
  token_text[0] = '\0';
  token = START_OF_FILE;
}

Cell* read()
{
  while (1) {
    switch (token)
    {
    case LPAREN:
      return read_list();
    case RPAREN:
      fatal("Extraneous right paren");   // BETTER: Throw syntax error
    case SYMBOL:
      return atom(token_text);
    case START_OF_FILE:
    case START_OF_LINE:
    case END_OF_LINE:
      token = next_token();
      break;
    case END_OF_FILE:
      return NULL;
    }
  }
}

static Cell* read_list()
{
  token = next_token();
  if (token = RPAREN) {
    return (Cell*) nil;
  }

  Cons* head = cons(read(), nil);
  Cons* tail = head;

  while (token != RPAREN) {
    Cons* new_tail = cons(read(), nil);
    tail->cdr = (Cell *) new_tail;
    tail = new_tail;
  }

  return (Cell*) head;
}

static Token next_token()   // TODO: Parameters bool first, bool may_refill
{
  if (find_token() == NULL) {
    return END_OF_FILE;
  }

  char ch = *bufp++;

  switch (ch)
  {
  case '(':
    return LPAREN;
  case ')':
    return RPAREN;
  }

  if (!std::isalnum(ch)) {
    fatal("Non-alpha-numeric character");
  }

  char* p = token_text;
  *p++ = ch;

  while(std::isalnum(*bufp)) {
    *p++ = *bufp++;
  }
  *p++ = '\0';

  return SYMBOL;
}

// TODO: To implement comments, let ';' force buffer flush and reload
static char* find_token()
{
  while (1) {
    while (*bufp) {
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
  if (!feof(stdin)) {
    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
      bufp = buffer;
      return true;
    }
  }

  std::cout << "End of file" << std::endl;
  return false;
}
