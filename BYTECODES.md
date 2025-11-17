# Notes for bytecode interpreter

## New cell types

Function<env, body>

Bytecode <oper><arg1, arg2>

## Interpreter globals

- ip = instruction pointer
- ep = enviroment pointer

## Environment structure

cons(list(caller_ep, caller_ip, caller_sp), list(local_1, local_2, ..., local_n))

## Operations

CONS
CAR
CDR
ATOM
EQ
IF<true_sexpr><false_sexpr>

PUSH_SEXPR <cellp>

PUSH_LOCAL <i>   - push ith local of the current environment
PUSH_VAR <i,j>  - jth variable of ith lexical environment
PUSH_GLOBAL <name>

An environment has these components:
- vector of local variables (constructed from args/formals pairing)
- (pointer to) lexical environment

Interpreter's global variables are:
- instruction pointer (ip)
- environment pointer (ep)
- stack frame pointer (fp)

A stack frame has these components:
- (pointer to) caller's next instruction
- (pointer to) caller's environment
- (pointer to) caller's stack frame

PUSH_SEXPR<env>
PUSH_SEXPR<body>
CREATE_CLOSURE

PUSH <closure>
PUSH <arg1>
. . .
PUSH <argN>
CALL <n>
   - construct new env from
      - args vector
      - lexical env (stored in closure)
   - save pointer to new env in temp
   - construct new stack frame on the stack
   - point interpreter's globals to new ip (from closure), env, stack frame

[RETURN
   - pop stack and save in temp
   - restore interpreter's global variables from stack frame
   - push temp result

READ
PRINT

## MVP increments

Implement
- push_sexpr

Tests
    push_sexpr nil
    <confirm nil on top>

    push_sexpr hello
    <confirm hello on top>

Implement
- cons
- car
- cdr

Test
    push_sexpr hello
    push_sexpr world
    cons
    car
    <confirm hello on top>

    push_sexpr hello
    push_sexpr world
    cons
    cdr
    <confirm world on top>
