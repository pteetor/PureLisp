//
//  strings and string heap
//

#include "doctest.h"

#include "consVM.h"

static const int N_BYTES = 1000 * 1000;

static char space[N_BYTES];
static char* frontier;
static int n_free;

static String* find_string(const char* p);
static int required_space(const char* s);
static int required_space(int len);

void init_strings()
{
    frontier = space;
    n_free = N_BYTES;
}

String* make_string(const char* s)
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
    p->length = len;
    std::memcpy(p->body, s, len);

    frontier += n;
    n_free -= n;
    return p;
}

static String* find_string(const char* p)
{
    // TBD
    return NULL;
}

static int required_space(const char* s)
{
    return required_space(std::strlen(s));
}

static int required_space(int len)
{
    int padded_len = sizeof(String) * (len + sizeof(String) - 1) / sizeof(String);
    return sizeof(String) + padded_len;
}
