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

PUSH_PTR <cellp>

PUSH_LOCAL <i>   - push ith local of the current environment

PUSH_ENV <i>   - push ith lexical environment
PUSH_VAR <j>  - jth variable of the environment on the top of stack

PUSH_PTR<env>
PUSH_PTR<body>
CREATE_FUNCTION

PUSH <fn>
PUSH <arg1>
. . .
PUSH <argN>
CALL <n>   - construct new env from ep, ip, sp, and top n pointers; pop <fn> into ep, ip

RETURN   - restore ip and ep from current env;

CONS
CAR
CDR
ATOM
EQ
IF_THEN_ELSE<true_ptr><false_ptr>

READ
PRINT

## MVP increments

Write
- push_ptr
- print

Test
    push_ptr list(hello world)
    print

Write
- cons

Test
    push_ptr hello
    push_ptr world
    push_ptr nil
    cons
    cons
    print

