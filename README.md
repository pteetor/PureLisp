# PureLisp

This is a bare-bones, MVP implementation of the pure LISP dialect
as described in the Lisp 1.5 Programmer's Manual.

The program is heavily inspired by Justine Tunney's "Sector Lisp".

### Building
Use `make` to build the executable.

```
$ make
```

### Interactive REPL
Running `consvm` gives you a basic REPL.

```
$ ./consvm
>> (quote (hello world))
s-expr: (quote (hello world))
=> (hello world)
```

Use Ctrl-D to exit.

### Unit testing
Use the Makefile to run the unit tests.

```
$ make run-tests
```

### Demos
This demo is a sort of regression test, showing some program capabilities.

```
$ ./consvm lisp/regression.lisp
```

This runs the remarkable "Lisp within Lisp" code of Justine Tunney.

```
$ ./consvm lisp/lisp.lisp
```
