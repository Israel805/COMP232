# CI LISP

Read **everything in this file** before opening the instructions for the individual tasks. You are responsible for understanding the grading structure before starting, and you will likely miss **tons of points** in progress checks if you do not.

## Summary

Through the course of this project, you will implement an interpreter for *CI LISP*, a *Lisp*-like language.

The project is divided into tasks, which should be done sequentially and are listed in the [Instructions](#instructions) section below.

## Progress Checks

We will do 10-minute one-on-one progress checks on the  dates listed below. The expected amount of progress in the project is listed for each progress check. Students making insufficient progress on a given progress check can be docked up to 12.5% on the project; students who miss progress checks will be docked the full 12.5%. Up to half of the lost points on a progress check can be recovered on the following progress check by catching back up. 

You will be graded from -12.5 to 0 on each progress check, and the progress check grades will be added into your final grade. Points will not be docked on the final progress check unless it is missed; anything that is not complete at the final check will be reflected in the project grade itself.

You will not necessarily be docked any points for not reaching the expected progress goals. If the expected tasks are near completion **and** you come prepared with questions and/or an organized plan detailing what you have left to do to reach the progress goals, you may receive full credit.

Whether you have reached the progress goals or not, you must come prepared with a summary of your progress. Moreover, your project github repository must be up to date with your project and include sample inputs demonstrating all implemented functionality (including functionality from prior progress checks). We will clone your project and run it with your inputs to verify. Students who meet progress goals but do not come prepared to show it will be docked points.

Tasks **must** be completed in order. A progress goal that states "Complete task N" really means "Complete tasks 1-N".

* `11/09` : Complete task 1.
* `11/18` : Complete task 2.
* `11/30` : Complete task 4.
* `12/11` : Complete the project.

## Submission Requirements

You final submission must come with:

* documentation summarizing what has and what has not been done.
* sample inputs demonstrating and testing all implemented functionality.
* a list of any known bugs or issues, and your theories about their causes.

These should all be in your project github repository before the final progress check, and you should come to said progress check prepared to discuss these materials and demonstrate a run with your sample inputs while screen sharing. The inputs should be in a file and ready to roll; **do not** come to a progress check expecting to type in inputs by hand while we watch, expecting us to type in inputs by hand, or expecting us to come up with test inputs. Come prepared.

## General Comments

The task 1 instructions include figures demonstrating how the syntax tree will be built for the initial grammar. You are encouraged to create similar visualizations of productions added in future tasks; these diagrams do not need to be submitted, but they will make it much easier to keep track of everything that's happening as the project gets larger and more complex. They will also better inform any questions you might have, and greatly increase the efficiency with which these questions are conveyed.

The task 1 instructions are much more detailed in their description of how to go about organizing the project than subsequent instructions. This is to give you a feel for how we expect the project to be organized as you progress. While this level of direction will not be given in the subsequent tasks, you should aim to keep everything as comparmentalized, clean, and easy to expand as task 1. Keep the [DRY](https://en.wikipedia.org/wiki/Don%27t_repeat_yourself) principles in mind as you work:

__*"Don't repeat yourself (DRY, or sometimes do not repeat yourself) is a principle of software development aimed at reducing repetition of software patterns, replacing it with abstractions or using data normalization to avoid redundancy.*__

__*Violations of DRY are typically referred to as WET solutions, which is commonly taken to stand for "write every time", "write everything twice", "we enjoy typing" or "waste everyone's time". WET solutions are common in multi-tiered architectures where a developer may be tasked with, for example, adding a comment field on a form in a web application. The text string "comment" might be repeated in the label, the HTML tag, in a read function name, a private variable, database DDL, queries, and so on. A DRY approach eliminates that redundancy by using frameworks that reduce or eliminate all those editing tasks except the most important ones, leaving the extensibility of adding new knowledge variables in one place.
"*__

It is a good idea to sweep each task 2-3 times after completion making it cleaner, replacing repeated code with function calls, and generally simplifying wherever possible. This will save you time in the long run, as simpler, more organized code is drastically easier to build on. There is already at least one violation of DRY in the handout; can you find it and fix it?

## <a name="instructions"></a> Instructions

Each task below task will expand the grammar; your job is to expand the lexer, parser, data structures, parsing methods, and evaluation methods to meet the new requirements. The handout will build and run, but isn't capable of actually doing anything other than crashing or quitting.

Click the hyperlinked task names to open their instructions.

[TASK 1](./instructions/task_1.md) : Tokenization, Parsing and Evaluation of arithmetics in Cambridge Polish Notation

[TASK 2](./instructions/task_2.md) : Symbol definition and evaluation

[TASK 3](./instructions/task_3.md) : Typecasting

[TASK 4](./instructions/task_4.md) : Conditionals, comparisons, read, random and print

[TASK 5](./instructions/task_5.md) : Composable, recursion-capable user-defined functions

## Point Breakdown

* Task 1 : 50 points
* Task 2 : 15 points
* Task 3 : 5 points
* Task 4 : 15 points
* Task 5 : 15 points
# COMP232
