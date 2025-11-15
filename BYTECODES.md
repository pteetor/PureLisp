# Bytecodes for ConsVM 

PUSH_PTR <cellp>

PUSH_LOCAL <i>   - push ith variable of the current environment

PUSH_ENV <i>   - push ith lexical environment
PUSH_VAR <j>  - jth variable of the environment on the top of stack

CALL <n>   - pop n from stack to create new environment; save pc, fp in new env; pop <bc_ptr> into pc; save new env in fp
RETURN   - restore pc and fp from current env;

CONS
CAR
CDR
ATOM
EQ
BRANCH <bc_ptr>
BRANCH_IF_TRUE <bc_ptr>
BRANCH_IF_FALSE <bc_ptr>
