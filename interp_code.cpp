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
    return;
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
            case Opcode::PRINT:
                print(top());
                drop(1);
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

TEST_CASE("return nil") {
    instr(Opcode::PUSH_SEXPR, nil, nil);
    push(nil);
    cons();
    interp_code();

    REQUIRE(is_nil(top()));
    drop(1);
}


// THIS IS NOT WORKING
TEST_CASE("return (quote (hello world))") {
    // Sythesize program:
    //   - (push_expr (quote (hello world)))
    //   - (println)
    push(atom("hello"));
    push(atom("world"));
    push(nil);
    cons();
    cons();

    push(nil);
    instr(Opcode::PUSH_SEXPR);
    cons();

    interp_code();
    print(top());
    drop(1);
}
