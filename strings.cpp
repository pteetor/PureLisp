//
//  strings and string heap
//

#include "doctest.h"

#include "consVM.h"

static const int N_BYTES = 1000 * 1000;

static char space[N_BYTES];
static char* frontier;
static int n_free;
static String* string_list;

static String* find_string(const char* p);
static int required_space(const char* s);
static int required_space(int len);

void init_strings()
{
    frontier = space;
    n_free = N_BYTES;
    string_list = NULL;
}

void print(String* s)
{
  std::cout.write(s->body, s->length);
}

String* intern_string(const char* s)
{
    String* p = find_string(s);
    if (p != NULL)
    {
        return p;
    }

    int len = std::strlen(s);

    int n = required_space(len);
    if (n > n_free) {
        LispError("String space exhausted", true);
    }

    p = (String*) frontier;
    p->next = string_list;
    p->length = len;
    p->flags = 0;
    std::strcpy(p->body, s);

    frontier += n;
    n_free -= n;
    string_list = p;
    return p;
}

static String* find_string(const char* s)
{
  int len = std::strlen(s);
  String* p = string_list;
  while (p != NULL)
  {
    if (len == p->length && std::strcmp(s, p->body) == 0) {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

static int required_space(const char* s)
{
    return required_space(std::strlen(s) + 1);   // Account for '\0 terminator
}

static int required_space(int len)
{
    int padded_len = sizeof(String) * (len + sizeof(String) - 1) / sizeof(String);
    return sizeof(String) + padded_len;
}

// -------------------------------------

TEST_CASE("Strings are correctly stored") {
  const char* s = "test case";
  String* p = intern_string(s);
  REQUIRE(std::strcmp(p->body, s) == 0);
}

TEST_CASE("Strings are unique") {
  String* p = intern_string("foo");
  String* q = intern_string("fum");
  String* r = intern_string("foo");

  REQUIRE(p != q);
  REQUIRE(p == r);
}
