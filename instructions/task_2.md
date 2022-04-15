# TASK 2

## SUMMARY

Here you will add support for definition and evaluation of variables (called **symbols** in Lisp jargon). We'll add the following productions to the grammar:

```
s_expr ::= SYMBOL | ( let_section s_expr )

let_section ::= ( let let_list )

let_list ::= let_elem | let_elem let_list

let_elem ::= ( SYMBOL s_expr )

SYMBOL ::= at least 1 letter,
	followed by 0 or more letters and digits
	
letter ::= lowercase english alphabet letter
	| uppercase english alphabet letter
	| $ | _
```

You will need to update your project to support these additions.

While implementing the new productions above in your yacc file, be careful **not** to:

* allow `let_section`s to be empty (this in turn allows for redundant parenthesis, which will create parser conflicts when we implement 0-argument functions later on in the project).
* do left-recursion when making `let_list`s; this will create shift-reduce conflicts. In this case yacc can handle them, but still, never do left-recursion. In fact, you might want to go back and check your task 1 implementation to ensure that you didn't do left-recursion for `s_expr_lists`s.

## LEXING

Symbol tokenization should be a walk in the park; just make sure it comes after the function and keyword tokenizations in the lex file so that functions and keywords aren't tokenized as symbols!

In addition to symbol tokens, there is also a new keyword to add to your lexer.

## DATA STRUCTURES

### NEW NODE TYPES

First, you'll need a way to store symbols in **AST\_NODE**s, just like you have already stored numbers and functions. You'll want to add two new elements to the **AST_NODE_TYPE** enum and create corresponding structs to store symbol data and the scope data in the syntax tree.

```c
typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYM_NODE_TYPE,
    SCOPE_NODE_TYPE
} AST_NODE_TYPE;

typedef struct {
    char* id;
} AST_SYMBOL;

typedef struct {
    struct ast_node *child;
} AST_SCOPE;
```

### SYMBOL DEFINITIONS

A `let_elem` defines a symbol. Its children are a `SYMBOL` token and an `s_expr`. The idea is to assign value to a variable using the identifier stored in the `SYMBOL` token and value of the `AST_NODE` corresponding to the `s_expr`.

You should use a linked list of symbols in each scope to keep track of the symbol names alongside their values. You may want to use this struct (or something similar) for this linked list:

```c
typedef struct symbol_table_node {
    char *id;
    AST_NODE *value;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;
```

### LINKING DEFINITIONS TO EXPRESSIONS

Finally, to link each scope to its contained symbol table and to parent scopes, and to allow **AST\_SYMBOL**s to be stored in generic **AST\_NODE**s you'll want to modify the **AST\_NODE** struct:

```c
typedef struct ast_node {
    AST_NODE_TYPE type;
    struct ast_node *parent;
    struct symbol_table_node *symbolTable;
    union {
        AST_NUMBER number;
        AST_FUNCTION function;
        AST_SYMBOL symbol;
        AST_SCOPE scope;
    } data;
    struct ast_node *next;
} AST_NODE;
```

### SCOPING

Scope nodes will be the result of the `s_expr ::= ( let_section s_expr )` production; they should be minimal nodes, which store a reference to the child `s_expr` (the one to the right of the `let_section`) from the parent node (the one to the left).

Before we continue this discussion of scoping, it is important to clarify that every **AST\_NODE** defines / contains a scope. **AST\_SCOPE** nodes aren't special in this regard; to the contrary, they're special because they're **only** a scope.

Looking at the `s_expr ::= ( let_section s_expr )` production, it may not be readily apparent why scope nodes are necessary; why not just let the the `s_expr` on the left literally BE the `s_expr` on the right once the `let_section` has been referenced to store variable definitions. In other words, why not let this scope "collapse" into the child `s_expr` once it has been used to assign the `let_section` to that `s_expr`? To see why, consider an input in the form `( let_section ( let_section s_expr ) )`; without the use of a scope node, these two `let_section`s would have to be combined into a single `let_section` (i.e. into a single scope). But there are two scopes here! Symbols defined in the inner `let_section` should not be accessible in the outer `let_section`. For a more tangible example, check [this input and explanation](#scope_example) below.

### SUBROUTINES

You will need to add methods for **AST\_NODE** creationg for both new types (**SCOPE\_NODE\_TYPE** and **SYMBOL\_NODE\_TYPE**), for linking symbol table nodes together into linked lists, for linking said linked lists of symbol table nodes up to the **AST\_NODE**s whose scope they are in, and for looking through these symbol tables to find symbol definitions in order to evaluate them.

When evaluating symbols, you need to look not only through the local symbol table but also through the symbol tables in parent scopes; this is the purpose of the **parent** pointer added to **AST\_NODE**s.

If an undefined variable is evaluated, a warning should be printed the **NAN\_RET\_VAL** should be returned. Symbol evaluation should look something like this:

```
current scope <- the ast_node being evaluated (a symbol node storing an identifier)

while the current scope is not null
{
	Go through the symbol table associated with the current scope.
	Check each symbol to see if it has the identifier you're looking for. 
	eval and return if found.
	
	Set the current scope to its parent scope.
}

The variable was undefined, print a warning and return NAN.
```

You will need to assign these parent scopes in several of places:

* When the `s_expr ::= ( let_section s_expr )` production is performed:
	* The child scope (the `s_expr` on the right) must have its parent set to the parent scope (the `s_expr` on the left).
	* The child scope must have its symbol table assigned to the `let_section`.
	* The `s_expr`s stored in the `let_section` (i.e. the values of its defined symbols) must have their parent set to the child scope.
* When the `f_expr ::= ( FUNC s_expr_section )` production is performed, the `s_expr`s in the `let_section` should have their parent set to the `f_expr` (i.e. the parent of an operand is the function call containing it).

Whenever a symbol is evaluated, its value in the symbol table will be stored as an **AST\_NODE**. That **AST\_NODE** should be evaluated (whatever type it is). Then, if the **AST\_NODE** is not a number node, it should be replaced with a number node storing the value which resulted from evaluating the original **AST\_NODE**. This means that if a variable is accessed twice, the value found on the first evaluation will be used on the second evaluation; work will not be repeated and the value won't change if other variables change. Don't forget to call **freeNode** on the **AST\_NODE** being replaced!

There are some specifications to be made with respect to how symbols and scoping should behave, and we will make them through examples discussed below. All of these inputs are in the task 2 test input, and included in the [sample run](#sample_run) (in order, for the most part, but with some others between them).

## <a></a>

```
( (let (x 1) (x 2)) x )
```

Here the symbol `x` is defined twice in the same scope. The second definition should be discarded, (and a warning should be printed stating as much) and the output of the access to `x` should be `1`.

## <a></a>

```
( (let (x 1)) ( (let (x 2)) x ) )
```

`x` is defined in two different (nested) scopes, and then accessed in the inner scope. The resuling value should be `2`, from evaluating the **more local** definition.

## <a></a>

```
( (let (x 1)) ( (let (y 2)) (add x y) ) )
```

`x` and `y` are defined in different (nested) scopes. They should both be accessible where the addition call is made, becase it is in both scopes, and the result should be `3`.

## <a></a>

```
( (let (a ( (let (b 2)) (add b 3) ))) a )
```

`a`'s let element contains a nested definition of `b`, and accesses it. `a` is given then value `5` (the result of adding `3` to `b`). There are no issues here; no variables are accessed outside of the scope in which they are defined.

## <a></a>

```
( (let (a ( (let (b 2)) (add b 3) ))) (add a b) )
```

`b` is defined within the let element defining `a`. `b` is accessed, but outside of this let element (i.e. outside of the scope in which it is defined). `b` is undefined where it is accessed. A warning should be printed (undefined variable) and the attempted access to `b` should result in a **NAN\_RET\_VAL**.

## <a name="scope_example"></a>

```
( (let (a 1) (b a)) ((let (a 2)) b ) )
```

In the input above, `a` is defined to be `1`, and in the same scope `b` is defined to be `a`; in the scope in which `b` is accessed, `a` has been redefined to `2`.

Evaluating `b` requires first finding `b`'s definition (in the `let_section` on the left) and then evaluating the `s_expr` assigned to `b` (in this case, the symbol `a`). While the assignment of `a` to `2` is accessible to `b`, it is not accessible to the `let_elem` defining `b`, which is in an outer scope. The outer definition for `a`, however, is accessible to `b`'s `let_elem`; `b` should be `1`.

For the same reason, the input below should result in an undefined symbol warning and a **NAN\_RET\_VAL** output. The undefined symbol is the `a` within `b`'s let element.

```
( (let (b a)) ((let (a 2)) b ) )
```

## <a name="sample_run"></a> SAMPLE RUN

Below is a sample run with the provided task 2 test input. These test cases should be used for initial debugging; the test inputs which will be used when grading will be qualitatively similar in their required functionality, but will be larger, more complex, deeper (in terms of parse tree depth, or depth of nested scopes if you prefer), and more varied in terms of the numeric inputs, functions, and identifiers used. The identifiers below are particularly bad for testing the symbol definition in the grammar; be sure to test with identifiers including digits, underscores, and dollar signs! In other words, these test the basic functionality for this task, but do little in terms of stress-testing or checking integration with task 1.

TLDR: Once your implementation matches this sample run for the provided task 2 inputs, you should create better test cases to more fully test the entire project so far.

```
> x
WARNING: Undefined Symbol "x" evaluated! NAN returned!
Double : nan

> ( (let (x 1) ) x )
Integer : 1

> ( (let (x 1) (x 2)) x )
WARNING: Duplicate assignment to symbol "x" detected in the same scope!
Only the first assignment is kept!
Integer : 1

> ( (let (x 1)) ( (let (x 2)) x ) )
Integer : 2

> ( (let (x 1)) ( (let (y 2)) (add x y) ) )
Integer : 3

> ( (let (a ( (let (b 2)) (add b 3) ))) a )
Integer : 5

> ( (let (a ( (let (b 2)) (add b 3) ))) (add a b) )
WARNING: Undefined Symbol "b" evaluated! NAN returned!
Double : nan

> ( (let (a 1) (b a)) ((let (a 2)) b ) )
Integer : 1

> ( (let (b a)) ((let (a 2)) b ) )
WARNING: Undefined Symbol "a" evaluated! NAN returned!
Double : nan

> ( (let (a 1) (b a)) ((let (c ( (let (a 2)) a ))) b ) )
Integer : 1

> ( (let (y 1) (x y)) ( (let (y 2) (c x)) x ) )
Integer : 1

> (add ((let (abc 1)) (sub 3 abc)) 4)
Integer : 6

> (mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
Double : 4.242641

> (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
Double : 5.398346

> ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
Double : 17.414214

> ((let (abc 1)) (sub ((let (abc 2) (de 3)) (add abc de)) abc))
Integer : 4

> quit

Process finished with exit code 0
```

