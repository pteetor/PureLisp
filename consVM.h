//
//  ConsVM definitions
//

#include <stddef.h>
#include <iostream>
#include <cstring>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <stdexcept>

class LispError : public std::runtime_error {
public:
  bool is_fatal;
  LispError(const std::string& message, bool is_fatal = false)
    : std::runtime_error(message), is_fatal(is_fatal) {}
};

// Cell definitions ----

enum class Tag : uint8_t {
  FREE_TAG = 0,
    ATOM_TAG = 1,
    CONS_TAG = 2,
    STRING_TAG = 3,
    INSTR = 4
};

enum class Opcode : uint8_t {
  NOP = 0,
    ATOM,
    CAR,
    CDR,
    CONS,
    EQ,
    IF,
    PRINT,
    PRINTLN,
    PUSH_SEXPR
};

struct Cell
{
  Tag type;
  Opcode oper;        // used only for type == INSTR
  uint16_t flags;
};

const uint16_t MARK_FLAG = 1 << 0;

inline bool is_marked(Cell* p) { return (p->flags & MARK_FLAG) != 0; }
inline bool not_marked(Cell* p) { return (p->flags & MARK_FLAG) == 0; }
inline void set_mark(Cell* p) { p->flags |= MARK_FLAG; }
inline void clear_mark(Cell* p) { p->flags &= ~MARK_FLAG; }

// Subclasses of Cell ----

struct String;
struct Atom;
struct Cons;
struct Instr;
struct Free;

struct String: public Cell
{
  String* forward;
  int length;
  char body[0];
};

struct Atom: public Cell
{
  Atom* next;
  String* string;
};


struct Cons: public Cell
{
  Cell* car;
  Cell* cdr;
};

// One instruction in the virrtual machine
struct Instr: public Cell
{
  union {
  int n1;
  Cell* opand1;
};
  Cell* opand2;
};

// Must have: sizeof(Cons) == sizeof(Atom) == sizeof(Instr) == sizeof(Free)
struct Free: public Cell
{
  Free* next;
  void* _unused;
};

inline bool is_string(Cell* p) { return (p->type == Tag::STRING_TAG); }
inline bool is_atom(Cell* p) { return (p->type == Tag::ATOM_TAG); }
inline bool is_cons(Cell* p) { return (p->type == Tag::CONS_TAG); }
inline bool is_instr(Cell* p) { return p->type == Tag::INSTR; }

//
// Global atoms
//
extern Atom* nil;
extern Atom *a_t, *a_quote, *a_cond, *a_atom,
*a_car, *a_cdr, *a_cons, *a_eq,
*a_lambda;

inline bool is_nil(Cell* p)  { return p == nil; }
inline bool non_nil(Cell* p) { return p != nil; }

// Pre-defined functions are stored in global_env
extern Cell* global_env;

extern bool tracing;

//
// Global functions ----
//
extern void cons();
extern void cons(Cell* car, Cell* cdr);
extern Cell* car(Cell* p);
extern Cell* cdr(Cell* p);
extern void print(Cons* p);
extern Cons* as_cons(Cell* p);
extern void audit_cons();

extern void init_atoms();
extern Atom* atom(const char* p);
extern void print(Atom* p);
extern Atom* as_atom(Cell* p);
extern void reset_chain();
extern void insert_into_chain(Atom* p);
extern void audit_atoms();

extern void instr(Opcode oper, Cell* opand1 , Cell* opand2);
extern void instr(Opcode oper);
extern void print(Instr* in);

extern void init_strings();
extern String* intern_string(const char* s);
extern void print(String* s);
extern void audit_strings();

extern void init_stack();
extern Cell* top();
extern void push(Cell*);
extern Cell* pop();
extern Cell* down(int n);
extern void drop(int n);
extern void collapse(int n);

extern void print(Cell* p);
extern void println(Cell* p);

extern void build_globals();
extern void eval();
extern void eval(Cell* a, Cell* e);

extern bool is_true(Cell*);
extern void make_list(int n);
extern void init_tracing();
extern void trace(const char* tag,
                  Cell* cell = NULL, Cell* cell2 = NULL);
extern void validate_cell_ptr(Cell* p);

extern bool read(bool top_level = false);

struct GCStatus {
  int heap_size;
  int n_marked;
  int n_recovered;

  GCStatus(int hs, int nm, int nr) :
    heap_size(hs), n_marked(nm), n_recovered(nr) {}
};

extern void init_heap();
extern Atom* alloc_atom();
extern Cons* alloc_cons();
extern Instr* alloc_instr();
extern GCStatus gc(const char* context = NULL);
extern int mark(Cell* p);
extern int mark_stack();
extern int sweep_strings();
extern void compactify_strings();

inline void init_consvm()
{
  init_stack();
  init_strings();
  init_heap();
  init_atoms();
  build_globals();
}
