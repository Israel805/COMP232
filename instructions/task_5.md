# TASK 5

## SUMMARY, LEXING AND PARSING

Your last task is add support for user-defined functions via lambda expressions. These functions will work with composition with eachother and with built-in functions. They will also work with recursion.

The final additions to the grammar will be:

```
f_expr ::= ( SYMBOL s_expr_section )

arg_list ::= SYMBOL | SYMBOL arg_list

let_elem ::= ( SYMBOL LAMBDA ( arg_list ) s_expr )
	| ( TYPE SYMBOL LAMBDA ( arg_list ) s_expr )
	
LAMBDA ::= "lambda"
```

An example `let_elem` defining a function could be:

```
( int integerAdd lambda (x y) (add x y) )
```

The function defined above is called `integerAdd`. It takes 2 arguments, specified in its `arg_list`, called `x` and `y`. It returns the result of addition of `x` and `y`, but cast to an integer.

## DATA STRUCTURES

Custom functions will need identifiers; you'll need to add identifier storage to the **AST_FUNCTION** struct for this purpose.

Function definitions will be stored in a symbol table. You may decide whether to store them in the same symbol table as variables or a separate table just for function definitions; I personally chose to do the prior.

In either case, you will need to expand the `SYMBOL_TABLE_NODE` to specify which type of symbol is being stored. Prior to this task, there was only one symbol type; we called it "symbol", but from here on out we'll refere to it as "variable". There are now two additional symbol types: lambda and argument. All three types of symbols will need to be treated differently.

It is strongly recommended that arguments are placed in their own symbol table for the following reasons: 

* They should be checked last when looking up symbols, so local variables can be made to "reassign" arguments within a custom function's body.
* Values will need to be added to and removed from arguments in order; this is easier if you don't have to go through a list containing variable and function definitions to find the argument entries.

In order for custom functions to work recursively, you will need to implement a stack on which the argument values will be stored (arguments will not use the **value** field in the symbol table). The final data structures are below:

```c
typedef enum {
    VAR_TYPE,
    LAMBDA_TYPE,
    ARG_TYPE
} SYMBOL_TYPE;

typedef struct symbol_table_node {
    char *id;
    AST_NODE *value;
    SYMBOL_TYPE symbolType;
    NUM_TYPE numType;
    struct stack_node *stack;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

typedef struct stack_node {
    RET_VAL value;
    struct stack_node *next;
} STACK_NODE;
```

## CUSTOM FUNCTION EVALUATION

Evaluating a custom function should be done as follows:

1. Evaluate all arguments. (You can temporarily stick their stack nodes together in a linked-list fashion).
2. Put all argument values on the stacks of their respective **ARG_TYPE** definitions in the the custom function's argument table.
3. Evaluate the custom function definition, using the values on the tops of the stacks when arguments are accessed. Store the result.
4. Pop the argument values off of the tops of their stacks.
5. Return the result.

Straying from the sequence of instructions above may result in functions which work on their own, but do not work recursively and/or with composition.

Note that a custom function and a variable may be defined in the same scope with the same identifier, because the parenthesis in `f_expr` syntax will make it clear which is being used. This means that when you are looking up a symbol for evaluation, you must ensure that the definition has not only the right identifier, but also the right symbol type; when looking up a function symbol, the defining symbol table node's type should be **LAMBDA\_TYPE**, but when looking up a variable symbol the definition can come from either a **VAR\_TYPE** or an **ARG\_TYPE**.

Be sure to **not** replace custom function definitions with numeric values, as was done with variable evaluation.

## Sample Run

The inputs for sample run below are provided in the task 5 sample input. As always, you should generated more complete composite tests. Depending on what markdown editor you're using, you'll likely need to scroll right to see the end of the last input (or just open the sample input file).

```
> ( (let (int integerAdd lambda (x y) (add x y))) (integerAdd 1.495 1.495) )
WARNING: Precision loss on int cast from 2.99 to 2.
Integer : 2

> ( (let (x 1.495) (int x lambda (a b) (add a b))) (x x x) )
WARNING: Precision loss on int cast from 2.99 to 2.
Integer : 2

> ( (let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2) )
Double : 22.000000

> ( (let (f lambda (x y) (add x y))) (f (sub 5 2) (mult 2 3)) )
Integer : 9

> ( (let (int a 1) (f lambda (x y) (add x y))) (f 2 (f a 3)) )
Integer : 6

> ( (let (x lambda () (rand))) (add (print (x)) (print (x))) )
Double : 0.000008
Double : 0.131538
Double : 0.131546

> ((let (double gcd lambda (x y) (cond (greater y x) (gcd y x) (cond (equal y 0) x (gcd y (remainder x y)))))) (gcd 95.5 55))
Double : 0.500000

> quit

Process finished with exit code 0
```


