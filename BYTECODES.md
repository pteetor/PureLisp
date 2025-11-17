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
IF_THEN_ELSE<true_ptr><false_ptr>

PUSH_SEXPR <cellp>

PUSH_LOCAL <i>   - push ith local of the current environment

PUSH_ENV <i>   - push ith lexical environment
PUSH_VAR <j>  - jth variable of the environment on the top of stack
PUSH_GLOBAL <name>

PUSH_SEXPR<env>
PUSH_SEXPR<body>
CREATE_CLOSURE

PUSH <closure>
PUSH <arg1>
. . .
PUSH <argN>
CALL <n>   - construct new env from ep, ip, sp, and top n pointers; pop <fn> into ep, ip

RETURN   - restore ip and ep from current env; implied by end of function body

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
