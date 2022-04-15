# TASK 4

## SUMMARY

In this task, you will:

* Implement the **rand**, **read**, **equal**, **less**, **greater** and **print** functions.
* Implement conditionals (CI Lisp's equivalent of the ternary operator)

## FUNCTION SPECIFICATIONS

The functions will work as follows:

* **rand**
	* no arguments
	* generates and returns random double from `0` to `1`
	* You may use C's **rand** function (which has "limited randomness", i.e. if left unseeded it will output the same sequence of values on every run), but you must normalize the answer to be between 0 and 1 exactly.
	* `man rand` to learn more
* **read**
	* No arguments.
	* Prints `read :: ` to the `stdout`, and gets the user's response from the **read_target** file.
	* The **read_target** is already set up for you; it is declared in the header file and initialized in the main. 
		* If you enter a second argument in the run configurations, it will be used as a file path to open the **read_target**.
		* If no second argument is provided, the **read_target** will be the console, so these inputs will be typed by hand (as was done in the sample runs).
	* If the user's response is an integer or double literal, as defined by CI Lisp's grammar, **read** returns the corresponding duck-typed number.
	* If the user's response is not a valid CILisp number literal, **read** should print a warning and return **NAN\_RET\_VAL**.
* **equal**, **less**, **greater**
	* Binary.
	* Return an integer with value `1` if the condition holds (i.e. if the first argument is equal to, less than, greater than the second respectively). returns an `int` with value `0` otherwise.
	* `1` is "true" and `0` is "false".
	* **equal** should ignore type, and should only check if the two numbers' stored values are **exactly** equal; no error threshold for floating point rounding errors should be used.
* **print**
	* Unary.
	* Evaluates its operand.
	* Prints the result of evaluation with **printRetVal**, and then returns it.
	* This should be very simple, as **printRetVal** is already implemented.

## CONDITIONAL SPECIFICATIONS

The conditional operator will require adding the following to the grammar:

```
s_expr ::= ( COND s_expr s_expr s_expr )

COND ::= "cond"
```

You'll also need to add several more functions to the **FUNC** tokenization definition, the **FUNC\_TYPE** enum and **funcNames** array.

Conditionals can be implemented via adding a new **AST\_NODE\_TYPE** member and corresponding struct, and integrating this new type into the **AST\_NODE**. **Do not** implmenet conditionals by adding a new function instead; syntax errors should be thrown if a conditional doesn't have exactly 3 `s_expr`s.

If the first of the three `s_expr` (the condition) is nonzero (i.e. "true") when evaluated, then the conditional returns the value of the second `s_expr`. Otherwise, the condition is zero (i.e. "false") so the third `s_expr`'s value is returned.

If `A`, `B` and `C` are expressions, then CI Lisp's `( cond A B C )` is the equivalent of `A ? B : C` in C (the ternary operator).

## SAMPLE RUN

The inputs for the sample run are provided in the task 4 sample input. It demonstrates the new functionality in very simple inputs, and tests some of the old functionality. As always, you should construct better test inputs to ensure that the new functionality is fully integrated with the old. A file is also provided for read inputs which matches those in this run; running with this file as the **read_target** will lead to the same outputs (but it won't be quite as pretty).

Make sure you understand why each value is printed in the sample run. Note that the result of the full program evaluation is always printed, so the last printed value following each input is not from a call to the **print** function, but from the **printRetVal** calls in the `.y` file, in the `program` productions.

The last two inputs (not including the `quit`) in the sample run provide an opportunity to test specifications from task 2 which are often overlooked:

* In the second to last input, if `x`'s value changes between first two prints, then variable evaluation is not being done as specified. The value stored in the symbol table should be replaced with an **AST\_NODE** of type **NUMBER\_NODE\_TYPE** housing an **AST\_NUMBER** the first time `x` is evaluated, so the second evaluation of `x` should not run the **rand** function again. The third printed value below is, of course, the result of the entire expression, which is `(add x x)`.

* Similarly, in the last input, while `x` and `y` are both accessed twice, there is only a single **read** call for each one; if `x`'s definition is left as a function node containing a **read** call after the first evaluation, then **read** will be incorrectly called a second time for `x`, but if `x`'s definition is replaced with a numerical value the first time it is accessed, then the second access simply gets this numerical value. The same applies to `y`. As such, the last input should do two reads (one for `x` and one for `y`), not four reads.

```
> (print)
WARNING: print called with no operands!
Double : nan

> (print 1)
Integer : 1
Integer : 1

> (print 1 2)
WARNING: print called with extra (ignored) operands!
Integer : 1
Integer : 1

> (add 1 (print 2) )
Integer : 2
Integer : 3

> (rand)
Double : 0.000008

> (rand)
Double : 0.131538

> (rand)
Double : 0.755605

> (rand)
Double : 0.458650

> (rand)
Double : 0.532767

> (rand)
Double : 0.218959

> (rand)
Double : 0.047045

> (rand)
Double : 0.678865

> (rand)
Double : 0.679296

> (rand)
Double : 0.934693

> (read)
read :: 1
Integer : 1

> (read)
read :: asdf
WARNING: Invalid read entry! NAN returned!
Double : nan

> (read)
read :: .5
WARNING: Invalid read entry! NAN returned!
Double : nan

> (read)
read :: -5.5
Double : -5.500000

> (add (read) (read))
read :: 10
read :: -10
Integer : 0

> (equal 0 0)
Integer : 1

> (equal 0 0.0)
Integer : 1

> (equal 0 0.0001)
Integer : 0

> (less 0 0)
Integer : 0

> (less -1 0)
Integer : 1

> (less 0 -0.00001)
Integer : 0

> (greater 0 1)
Integer : 0

> (greater 1 0)
Integer : 1

> (greater 0 0.0)
Integer : 0

> ( ( let (x 0) (y 1) ) (less x y) )
Integer : 1

> (cond 0 5 6)
Integer : 6

> (cond 1 5 6)
Integer : 5

> (cond (less 0 1) 5 6)
Integer : 5

> (cond (less 1 0) 5 6)
Integer : 6

> ( ( let (x (read)) (y (read)) ) (add (print x) (print x) (print y) (print y)) )
read :: -17.2
Double : -17.200000
Double : -17.200000
read :: +127
Integer : 127
Integer : 127
Double : 219.600000

> ( ( let (x (rand)) ) (add (print x) (print x) ) )
Double : 0.383502
Double : 0.383502
Double : 0.767004

> quit

Process finished with exit code 0
```
