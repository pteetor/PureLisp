//
//  Interpreter for bytocodes
//

#include "doctest.h"
#include "consVM.h"

void instr(Opcode oper, Cell* opand1 , Cell* opand2)
{
    push(opand1);
    push(opand2);
    instr(oper);
}

void instr(Opcode oper)
{
    Instr* p = alloc_instr();
    p->oper = oper;
    p->opand2 = pop();
    p->opand1 = pop();
    push(p);
}

void print(Instr* in)
{
    std::cout << "[instr " << (int) in->oper << " " << in->opand1 << " " << in->opand2 << "]" <<  std::endl;
}

//
// Interpreter global variables
//
Cons* ip;     // Instruction pointer
Cons* env;    // Environment pointer

void interp_code()
{
    Instr* instr;

    ip = (Cons*) top();
    while (non_nil(ip)) {
        instr = (Instr*) ip->car;
        ip = (Cons*) ip->cdr;

        if (!is_instr(instr)) {
            throw LispError("interp_code: not an instr");
        }

        switch(instr->oper)
        {
            case Opcode::NOP:
                break;
            case Opcode::ATOM:
                push(is_atom(pop()) ? a_t : nil);
                break;
            case Opcode::CAR:
                push(car(pop()));
                break;
            case Opcode::CDR:
                push(cdr(pop()));
                break;
            case Opcode::CONS:
                cons();
                break;
            case Opcode::PRINT:
                print(top());
                drop(1);
                break;
            case Opcode::EQ:
                {
                    Cell* a = pop();
                    Cell* b = pop();
                    push(a == b ? a_t : nil);
                }
                break;
            case Opcode::IF:
                if (is_true(pop())) {
                    ip = (Cons*) (instr->opand1);
                } else {
                    ip = (Cons*) (instr->opand2);
                }
                break;
            case Opcode::PRINTLN:
                println(top());
                drop(1);
                break;
            case Opcode::PUSH_SEXPR:
                push(instr->opand1);
                break;
            default:
                LispError("interp_code: unimplemented opcode");
        }
    }

    collapse(1);
}

// -------------------------------------

//
//  Unit tests
//

TEST_CASE("bytecode for push nil") {
    instr(Opcode::PUSH_SEXPR, nil, nil);
    push(nil);
    cons();
    interp_code();

    REQUIRE(is_nil(top()));
    drop(1);
}

TEST_CASE("bytecode for push atom") {
    Atom* hello = atom("hello");

    instr(Opcode::PUSH_SEXPR, hello, nil);
    push(nil);
    cons();
    interp_code();

    REQUIRE(top() == hello);
    drop(1);
}

TEST_CASE("bytecode for cons, car, and cdr") {
    Atom* hello = atom("hello");
    Atom* world = atom("world");

    // Test CAR
    instr(Opcode::PUSH_SEXPR, hello, nil);
    instr(Opcode::PUSH_SEXPR, world, nil);
    instr(Opcode::CONS, nil, nil);
    instr(Opcode::CAR, nil, nil);
    make_list(4);
    interp_code();

    REQUIRE(top() == hello);
    drop(1);

    // Test CDR
    instr(Opcode::PUSH_SEXPR, hello, nil);
    instr(Opcode::PUSH_SEXPR, world, nil);
    instr(Opcode::CONS, nil, nil);
    instr(Opcode::CDR, nil, nil);
    make_list(4);
    interp_code();

    REQUIRE(top() == world);
    drop(1);
}

TEST_CASE("bytecode for atom predicate") {
  Atom* hello = atom("hello");
  Atom* world = atom("world");

  // ATOM predicate - true case
  instr(Opcode::PUSH_SEXPR, hello, nil);
  instr(Opcode::ATOM, nil, nil);
  make_list(2);
  interp_code();

  REQUIRE(is_true(top()));
  drop(1);

  // ATOM predicate - false case
  instr(Opcode::PUSH_SEXPR, hello, nil);
  instr(Opcode::PUSH_SEXPR, world, nil);
  instr(Opcode::CONS, nil, nil);
  instr(Opcode::ATOM, nil, nil);
  make_list(4);
  interp_code();

  REQUIRE(is_nil(top()));
  drop(1);
}

TEST_CASE("bytecode for eq predicate") {
  Atom* hello = atom("hello");
  push(hello);
  Atom* world = atom("world");
  push(world);

  // EQ predicate - true case
  instr(Opcode::PUSH_SEXPR, hello, nil);
  instr(Opcode::PUSH_SEXPR, hello, nil);
  instr(Opcode::EQ, nil, nil);
  make_list(3);
  interp_code();

  REQUIRE(is_true(top()));
  drop(1);

  // EQ predicate - false case
  instr(Opcode::PUSH_SEXPR, hello, nil);
  instr(Opcode::PUSH_SEXPR, world, nil);
  instr(Opcode::EQ, nil, nil);
  make_list(3);
  interp_code();

  REQUIRE(is_nil(top()));
  drop(1);

  drop(2);   // Drop atoms
}

TEST_CASE("if branches correctly") {
    Atom* correct = atom("correctBranch");
    push(correct);
    Atom* wrong = atom("wrongBranch");
    push(wrong);

    // "then" case
    instr(Opcode::PUSH_SEXPR, a_t, nil);

    instr(Opcode::PUSH_SEXPR, correct, nil);
    make_list(1);
    instr(Opcode::PUSH_SEXPR, wrong, nil);
    make_list(1);
    instr(Opcode::IF);

    make_list(2);
    interp_code();

    REQUIRE(top() == correct);
    drop(1);

    // "else" case
    instr(Opcode::PUSH_SEXPR, nil, nil);

    instr(Opcode::PUSH_SEXPR, wrong, nil);
    make_list(1);
    instr(Opcode::PUSH_SEXPR, correct, nil);
    make_list(1);
    instr(Opcode::IF);

    make_list(2);
    interp_code();

    REQUIRE(top() == correct);
    drop(1);

    drop(2);   // Drop atoms
}
