# TASK 3

In this task, you will add typecasting functionality to the `let_elem`, allowing symbols to have a specified type.

The new additions to the grammar are:

```
let_elem ::= ( TYPE SYMBOL s_expr )

TYPE ::= "int" | "double"
```

You'll need to expand the **SYMBOL\_TABLE\_NODE** to include a **type** field (storing a **NUM\_TYPE**). You should also expand the **NUM\_TYPE** enum to include a **NO\_TYPE** element, for when **SYMBOL\_TABLE\_NODE**s are created without type-casting. Numbers with **NO\_TYPE** should never exist; **NO\_TYPE** is solely a placeholder to say "this variable will take whatever type it was assigned, without casting it", i.e. the variable will be duck-typed.

When evaluating symbols, you'll need to take their type into consideration; the **AST\_NUMBER** (which should be stored in the **SYMBOL\_TABLE\_NODE**'s referenced value **AST\_NODE** after evaluation) should be stored with typecasting applied. If this causes a loss of precision (i.e. if a double is cast as an integer) then a warning should be printed stating that this is the case.

This task should be quick; if you find you need to make more than a few simple changes, you're likely overthinking it!

The sample run below coincides with the provided task 3 test input. This **only** tests the basic new functionality, and does not ensure that it is well integrated with the old functionality. You should create more complex test cases, testing the entire project so far.

```
> ((let (int a 1.25))(add a 1))
WARNING: Precision loss on int cast from 1.25 to 1.
Integer : 2

> ((let (double a 5))(add a 25))
Double : 30.000000

> ((let (double $_1 (add 1 2))) (mult $_1 3))
Double : 9.000000

> quit

Process finished with exit code 0
```
