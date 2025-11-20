//
//  strings and string heap
//

#include "doctest.h"

#include "consVM.h"

static const int N_BYTES = 1000 * 1000;

static char space[N_BYTES];
static String* frontier;
static int n_free;

static int required_space(const char* s);
static int required_space(String* s);
static int required_space(int len);
static String* next_string(String* s);
static String* find_string(const char* p);

void init_strings()
{
  frontier = (String*) space;
  n_free = N_BYTES;
}

void print(String* s)
{
  std::cout.write(s->body, s->length);
}

String* intern_string(const char* s)
{
  String* p = find_string(s);
  if (p != NULL) {
    return p;
  }

  int len = std::strlen(s);

  int n = required_space(len);
  if (n > n_free) {
    gc("string space exhausted");
    if (n > n_free) {
      LispError("String space exhausted", true);
    }
  }

  p = frontier;
  p->forward = p;
  p->length = len;
  p->flags = 0;
  std::strcpy(p->body, s);

  frontier = next_string(frontier);
  n_free -= n;
  return p;
}

static int required_space(const char* s)
{
  return required_space(std::strlen(s) + 1);   // Account for '\0 terminator
}

static int required_space(String* s)
{
  return required_space(s->length);
}

static int required_space(int len)
{
  int padded_len = sizeof(String) * (len + sizeof(String) - 1) / sizeof(String);
  return sizeof(String) + padded_len;
}

static String* next_string(String* s)
{
  return (String*) ((char*) s + required_space(s));
}

static String* find_string(const char* s)
{
  String* p = (String*) space;
  while (p < frontier)
  {
    if (std::strcmp(s, p->body) == 0) {
      return p;
    }
    p = next_string(p);
  }
  return NULL;
}

int sweep_strings()
{
  // Find the first not-marked string,
  // For marked strings, set forward = self (i.e., not moved)
  String* p = (String*) space;
  while (p < frontier) {
    if (not_marked(p)) {
      break;
    }
    p->forward = p;
    p = next_string(p);
  }

  // If all strings are marked, done
  if (p == frontier) {
    return 0;
  }

  // freep points to non-marked string
  char* freep = (char*) p;
  int nBytesRecovered = 0;

  // For all strings up to frontier,
  // if marked, update forward = freep
  // if not marked, freep += size of string
  while (p < frontier) {
    if (is_marked(p)) {
      p->forward = (String*) freep;
      freep += required_space(p);
    } else {
      nBytesRecovered += required_space(p);
    }
    p = next_string(p);
  }

  return nBytesRecovered;
}

void compactify_strings() {
  String* p = (String*) space;

  // Find the first not-marked string;
  // that is, the first avaiable free space
  while (p < frontier) {
    if (not_marked(p)) {
      break;
    }
    clear_mark(p);
    p = next_string(p);
  }

  String* q = p;
  p = next_string(p);

  // Relocate all subsequent marked strings
  while (p < frontier) {
    if (is_marked(p)) {
      clear_mark(p);
      if (p != p->forward) {

        // Sanity check
        if (p->forward != q) {
          LispError("compactify_strings: inconsistent forwarding address");
        }

        std::memcpy((char*) q, (char*) p, required_space(p));
      }
      q = next_string(q);
    }
    p = next_string(p);
  }

  frontier = q;
}

// -------------------------------------

void audit_strings()
{
  String* p = (String*) space;
  int nStrings = 0;

  while (p < frontier) {
    if (p->type != Tag::STRING_TAG) {
      LispError("audit_strings: bad tag");
    }
    if (p->length != std::strlen(p->body)) {
      LispError("audit_strings: bad length");
    }
    ++nStrings;
    p = next_string(p);
  }

  std::cout << "audit_strings: Verified " << nStrings << " strings" << std::endl;
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
