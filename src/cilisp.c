#include "cilisp.h"

#define RED             "\033[31m"
#define RESET_COLOR     "\033[0m"

/*
 * Israel Alonso
 * Professor Ryan
 * COMP 232
 * 11/25/2020
 * */

// yyerror:
// Something went so wrong that the whole program should crash.
// You should basically never call this unless an allocation fails.
// (see the "yyerror("Memory allocation failed!")" calls and do the same.
// This is basically printf, but red, with "\nERROR: " prepended, "\n" appended,
// and an "exit(1);" at the end to crash the program.
// It's called "yyerror" instead of "error" so the parser will use it for errors too.
void yyerror(char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 255, format, args);

    printf(RED "\nERROR: %s\nExiting...\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end (args);
    exit(1);
}

// warning:
// Something went mildly wrong (on the user-input level, probably)
// Let the user know what happened and what you're doing about it.
// Then, move on. No big deal, they can enter more inputs. ¯\_(ツ)_/¯
// You should use this pretty often:
//      too many arguments, let them know and ignore the extra
//      too few arguments, let them know and return NAN
//      invalid arguments, let them know and return NAN
//      many more uses to be added as we progress...
// This is basically printf, but red, and with "\nWARNING: " prepended and "\n" appended.
void warning(char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 255, format, args);

    printf(RED "WARNING: %s\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end (args);
}

void freeStackNode(STACK_NODE *stack);
void freeSymbolTableNode(SYMBOL_TABLE_NODE *node);

FUNC_TYPE resolveFunc(char *funcName)
{
    // Array of string values for function names.
    // Must be in sync with members of the FUNC_TYPE enum in order for resolveFunc to work.
    // For example, funcNames[NEG_FUNC] should be "neg"
    char *funcNames[] = {
            "neg",
            "abs",
            "add",
            // TODO complete the array
            // the empty string below must remain the last element
            "sub",
            "mult",
            "div",
            "remainder",
            "exp",
            "exp2",
            "pow",
            "log",
            "sqrt",
            "cbrt",
            "hypot",
            "max",
            "min",
            "rand",
            "read",
            "print",
            "equal",
            "less",
            "greater",
            ""
    };
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
        {
            return i;
        }
        i++;
    }
    return CUSTOM_FUNC;
}

NUM_TYPE resolveType(char *typeName)
{
    if(strcmp(typeName, "int") == 0)
        return INT_TYPE;

    if(strcmp(typeName, "double") == 0)
        return DOUBLE_TYPE;

    return NO_TYPE;
}

bool hasOperand(AST_NODE *node) //Helper's helper function
{
    if (node->data.function.opList == NULL)
    {
        switch (node->data.function.func) {
            case NEG_FUNC:
                warning("neg called with no operands! nan returned!");
                break;
            case ABS_FUNC:
                warning("abs called with no operands! nan returned!");
                break;
            case ADD_FUNC:
                warning("add called with no operands! 0 returned!");
                break;
            case SUB_FUNC:
                warning("sub called with no operands!");
                break;
            case MULT_FUNC:
                warning("mult called with no operands! 1 returned!");
                break;
            case DIV_FUNC:
                warning("div called with no operands! nan returned!");
                break;
            case REMAINDER_FUNC:
                warning("remainder called with no operands! nan returned!");
                break;
            case EXP_FUNC:
                warning("exp called with no operands! nan returned!");
                break;
            case EXP2_FUNC:
                warning("exp2 called with no operands! nan returned!");
                break;
            case POW_FUNC:
                warning("pow called with no operands! nan returned!");
                break;
            case LOG_FUNC:
                warning("log called with no operands! nan returned!");
                break;
            case SQRT_FUNC:
                warning("sqrt called with no operands! nan returned!");
                break;
            case CBRT_FUNC:
                warning("cbrt called with no operands! nan returned!");
                break;
            case HYPOT_FUNC:
                warning("hypot called with no operands! nan returned!");
                break;
            case MAX_FUNC:
                warning("max called with no operands! nan returned!");
                break;
            case MIN_FUNC:
                warning("min called with no operands! nan returned!");
                break;
            case PRINT_FUNC:
                warning("print called with no operands! nan returned!");
                break;
            case EQUAL_FUNC:
                warning("equal called with no operands! nan returned!");
                break;
            case LESS_FUNC:
                warning("less called with no operands! nan returned!");
                break;
            case GREATER_FUNC:
                warning("greater called with no operands! nan returned!");
                break;
            case CUSTOM_FUNC:
                warning("custom called with no operands! nan returned!");
                break;
            default:
                break;
        }
        return false;
    }
    return true;
}

bool hasOnlyOneOperand(AST_NODE *node) //Helper's helper function
{
    if(node->data.function.opList->next == NULL)
    {
        switch (node->data.function.func){
            case SUB_FUNC:
                warning("sub called with only one arg!");
                break;
            case DIV_FUNC:
                warning("div called with only one arg! nan returned!");
                break;
            case REMAINDER_FUNC:
                warning("remainder called with only one arg! nan returned!");
                break;
            case POW_FUNC:
                warning("pow called with only one operand! nan return!");
                break;
            case EQUAL_FUNC:
                warning("equal called with only one operand! nan return!");
                break;
            case LESS_FUNC:
                warning("less called with only one operand! nan return!");
                break;
            case GREATER_FUNC:
                warning("greater called with only one operand! nan return!");
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

void hasExtraOperands(AST_NODE *node) //Helper's helper function
{
    if (node->data.function.opList->next != NULL)
    {
        switch (node->data.function.func){
            case NEG_FUNC:
                warning("neg called with extra (ignored) operands!");
                break;
            case ABS_FUNC:
                warning("abs called with extra (ignored) operands!");
                break;
            case EXP_FUNC:
                warning("exp called with extra (ignored) operands!");
                break;
            case EXP2_FUNC:
                warning("exp2 called with extra (ignored) operands!");
                break;
            case LOG_FUNC:
                warning("log called with extra (ignored) operands!");
                break;
            case SQRT_FUNC:
                warning("sqrt called with extra (ignored) operands!");
                break;
            case CBRT_FUNC:
                warning("cbrt called with extra (ignored) operands!");
                break;
            case PRINT_FUNC:
                warning("print called with extra (ignored) operands!");
                break;
            default:
                break;
        }//end switch
    }

    if(node->data.function.opList->next && node->data.function.opList->next->next != NULL)
    {
        switch (node->data.function.func) {
            case SUB_FUNC:
                warning("sub called with extra (ignored) operands!");
                return;
            case DIV_FUNC:
                warning("div called with extra (ignored) operands!");
                break;
            case REMAINDER_FUNC:
                warning("remainder called with extra (ignored) operands!");
                break;
            case POW_FUNC:
                warning("pow called with extra (ignored) operands!");
                break;
            default:
                break;
        }
    }
}

void directOpListToNode(AST_NODE *node,AST_NODE *opList) //Helper's helper function
{
    if(opList == NULL)
    {
        //check if last elem is null
        return;
    }

    opList->parent = node;
    directOpListToNode(node,opList->next);
}

void addToStack(STACK_NODE *stack,AST_NODE *node) //Helper's helper function
{
    if(node != NULL)
    {
        stack->value = eval(node);
        node = node->next;

        while(node != NULL)//add all the values in the stack
        {
            stack->next = calloc(sizeof(STACK_NODE),1);
            stack = stack->next;
            stack->value = eval(node);
            node = node->next;
        }
    }else{
        freeStackNode(stack);
    }
}

void castIfThereIsAType(const NUM_TYPE *type,NUM_TYPE *otherType, double *val) //Helper's helper function
{
    if((*type) != NO_TYPE)
    {
        if((*type) == INT_TYPE && (*otherType) == DOUBLE_TYPE)
        {
            double nodeVal = *val;
            *val = trunc(nodeVal);
            warning("Precision loss on int cast from %.2f to %.0f",nodeVal, *val);
        }

        (*otherType) = (*type);
    }
}

SYMBOL_TABLE_NODE *findLambda(AST_NODE *node) //Helper's helper function
{
    AST_NODE *currentScope = node->parent;
    SYMBOL_TABLE_NODE *result = NULL;

    while(currentScope != NULL)
    {
        SYMBOL_TABLE_NODE *stScope = currentScope->symbolTable;
        while(stScope != NULL)
        {
            if(strcmp(node->data.function.id, stScope->id) == 0 && stScope->symbolType == LAMBDA_TYPE)
            {
                result = stScope;
            }
            stScope = stScope->next;
        }
        currentScope = currentScope->parent;
    }

    return result;
}

void popArgsFromStacks(SYMBOL_TABLE_NODE *node) //Helper helper's function
{
    while(node != NULL) //traverse through the whole argtable
    {
        STACK_NODE *stackPtr = node->stack;
        if(stackPtr != NULL)
        {
            //shifts the stack to point to the next element
            node->stack = stackPtr->next;
            free(stackPtr); //frees the top of the stack
            stackPtr = NULL;//sets it to null
        }
        node = node->next;
    }
}

AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // TODO complete the function
    // Populate "node", the AST_NODE * created above with the argument data.
    // node is a generic AST_NODE, don't forget to specify it is of type NUMBER_NODE
    node->type = NUM_NODE_TYPE;
    node->data.number.type = type;
    node->data.number.value = value;

    return node;
}

AST_NODE *createSymbolNode(char *symbolID)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // TODO complete the function
    // Populate the allocated AST_NODE *node's data
    node->type = SYM_NODE_TYPE;
    node->data.symbol.id = symbolID;

    return node;
}
AST_NODE *createScopeNode(SYMBOL_TABLE_NODE *symbolTableNode, AST_NODE *child)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // TODO complete the function
    // Populate the allocated AST_NODE *node's data
    node->type = SCOPE_NODE_TYPE;
    node->symbolTable = symbolTableNode;
    node->data.scope.child = child;
    child->parent = node;
    node->data.scope.child->parent = node;

    while(symbolTableNode != NULL)
    {
        //Direct all the parent of the symboltable to node
        symbolTableNode->value->parent = node;
        symbolTableNode = symbolTableNode->next;
    }

    return node;
}

SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *newSymbol, SYMBOL_TABLE_NODE *symbolList)
{
    bool foundDuplicate = false;
    SYMBOL_TABLE_NODE *list = symbolList;
    while (list != NULL && !foundDuplicate)
    {
        if (strcmp(newSymbol->id, list->id) == 0 && newSymbol->symbolType == list->symbolType)
        {
            warning("Duplicate assignment to symbol \"%s\" detected in the same scope!\n"
                    "Only the first assignment is kept!", newSymbol->id);
            foundDuplicate = true;

            freeNode(list->value);
            list->value = newSymbol->value;
            newSymbol->value = NULL;
        }

        list = list->next;
    }

    if(foundDuplicate)
    {
        freeSymbolTableNode(newSymbol);
        newSymbol = NULL;
        return symbolList;
    }else{
        newSymbol->next = symbolList;
    }

    return newSymbol;
}

SYMBOL_TABLE_NODE *addArgToList(char *newSymbol, SYMBOL_TABLE_NODE *symbolList)
{
    SYMBOL_TABLE_NODE *newArg = createArgNode(newSymbol);

    bool foundDuplicate = false;
    SYMBOL_TABLE_NODE *list = symbolList;
    while (list != NULL && !foundDuplicate)
    {
        if (strcmp(newArg->id, list->id) == 0 && newArg->symbolType == list->symbolType)
        {
            warning("Duplicate assignment to symbol \"%s\" detected in the same scope!\n"
                    "Only the first assignment is kept!", newArg->id);
            foundDuplicate = true;

            freeSymbolTableNode(newArg);
            newArg = NULL;
        }

        list = list->next;
    }

    if(foundDuplicate)
    {
        return symbolList;
    }else{
        newArg->next = symbolList;
    }

    return newArg;
}

SYMBOL_TABLE_NODE *createArgNode(char *symbol)
{
    SYMBOL_TABLE_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }
    node->id = symbol;
    node->symbolType = ARG_TYPE;
    return node;
}

SYMBOL_TABLE_NODE *createSymbolTableNode(NUM_TYPE type,SYMBOL_TYPE symbolType,char *symbol,SYMBOL_TABLE_NODE *arg,AST_NODE *expr)
{
    SYMBOL_TABLE_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // TODO complete the function
    // Populate the allocated AST_NODE *node's data
    node->id = symbol;
    node->value = expr;
    node->numType = type;
    node->symbolType = symbolType;

    if(symbolType == LAMBDA_TYPE)
    {
        //Store all args in the lambda
        node->value->argTable = arg;
    }

    return node;
}

AST_NODE* createCondNode(AST_NODE *expr1, AST_NODE *expr2, AST_NODE *expr3)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    node->type = COND_NODE_TYPE;
    node->data.function.opList = expr1;
    node->data.function.opList->next = expr2;
    node->data.function.opList->next->next = expr3;
    directOpListToNode(node,node->data.function.opList);

    return node;
}

AST_NODE *createFunctionNode(FUNC_TYPE func, char *symbol, AST_NODE *opList)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // TODO complete the function
    // Populate the allocated AST_NODE *node's data
    node->type = FUNC_NODE_TYPE;
    node->data.function.func = func;
    node->data.function.id = symbol;
    node->data.function.opList = opList;

    if(opList != NULL)
    {
        opList->parent = node;
        AST_NODE *temp = opList;

        //recursive method to let oplists' parent point to nodes
        directOpListToNode(node,temp);
    }

    return node;
}

AST_NODE *addExpressionToList(AST_NODE *newExpr, AST_NODE *exprList)
{
    newExpr->next = exprList;
    return newExpr;
}

//Symbol eval func
RET_VAL evalSymbolFunction(AST_NODE *node)
{
    AST_NODE *currentScope = node->parent;
    RET_VAL currentRes = NAN_RET_VAL;
    bool found = false;

    while(currentScope != NULL && !found)
    {
        SYMBOL_TABLE_NODE *stScope = currentScope->symbolTable;
        while(stScope != NULL)
        {
            if(strcmp(stScope->id,node->data.symbol.id) == 0 && stScope->symbolType != LAMBDA_TYPE)
            {
                currentRes = eval(stScope->value);
                castIfThereIsAType(&stScope->numType,&currentRes.type,&currentRes.value);

                if(stScope->value->type != NUM_NODE_TYPE)
                {
                    freeNode(stScope->value);//Replace numberNode
                    stScope->value = createNumberNode(currentRes.value,currentRes.type);
                }

                found = true;
            }
            stScope = stScope->next;
        }

        SYMBOL_TABLE_NODE *argScope = currentScope->argTable;
        while(argScope != NULL)
        {
            if(strcmp(argScope->id,node->data.symbol.id) == 0)
            {
                currentRes = argScope->stack->value;
                found = true;
            }
            argScope = argScope->next;
        }
        currentScope = currentScope->parent;
    }

    if(!found)
    {
        warning("Undefined Symbol \"%s\" evaluated! NAN returned!",node->data.symbol.id);
        return NAN_RET_VAL;
    }

    return currentRes;
}

RET_VAL evalCondFunction(AST_NODE *node)
{
    //Modified
    return eval(node->data.function.opList).value ?
    eval(node->data.function.opList->next) : eval(node->data.function.opList->next->next);
}

RET_VAL evalCustomFunction(AST_NODE *node)
{
    RET_VAL result = NAN_RET_VAL;

    //find lambda
    SYMBOL_TABLE_NODE *lambda = findLambda(node);
    if(!lambda)
        return result;

    STACK_NODE *stackNode = calloc(sizeof(STACK_NODE),1);
    AST_NODE *operation = node->data.function.opList;

    //Adding all the Custom function's operations to the stack
    addToStack(stackNode,operation);

   SYMBOL_TABLE_NODE *argTable = lambda->value->argTable;
   STACK_NODE *newStackNode;

    //Have the stack in their respective arguments
    while(argTable != NULL)
   {
       newStackNode = stackNode->next;
       stackNode->next = argTable->stack;
       argTable->stack = stackNode;

       argTable = argTable->next;
       stackNode = newStackNode;

        //if stack is empty & there is still data
       if(argTable != NULL && stackNode == NULL)
       {
           warning("custom called with less arguments than needed! nan returned!");
           return result;
       }
   }

    if(lambda->value->argTable && argTable == NULL && stackNode != NULL)
        warning("Had extra (ignored) arguments.");

    result = eval(lambda->value); //evaluate the lambda value

    popArgsFromStacks(lambda->value->argTable);

    //Cast the result to the lambda's number type
    castIfThereIsAType(&lambda->numType,&result.type,&result.value);

    return result;
}

//Adding all operations from FUNC
RET_VAL evalNegFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if it has extra operands
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);
    result.value *= -1;
    return result;
}

RET_VAL evalAbsFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if it has extra operands

    RET_VAL result = eval(node->data.function.opList);
    result.value = fabs(result.value);
    return result;
}

RET_VAL evalAddFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return ZERO_RET_VAL;
    }

    RET_VAL result = ZERO_RET_VAL;
    AST_NODE *ptr = node->data.function.opList;
    RET_VAL other;
    do {
        other = eval(ptr);
        result.value = result.value + other.value;
        if(other.type == DOUBLE_TYPE)
            result.type = DOUBLE_TYPE;

        ptr = ptr->next;
    }while(ptr != NULL);

    return result;
}
RET_VAL evalSubFunction(AST_NODE *node)
{
    //if not operand or has only one operand
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if more than 2 operand

    AST_NODE *currentVal = node->data.function.opList;
    RET_VAL result = ZERO_RET_VAL;
    RET_VAL otherVal = eval(currentVal->next);
    RET_VAL val = eval(currentVal);
    result.type = val.type || otherVal.type;
    result.value = val.value - otherVal.value;

    return result;
}

RET_VAL evalMultFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return ONE_RET_VAL;
    }

    RET_VAL result = ONE_RET_VAL;
    AST_NODE *ptr = node->data.function.opList;
    RET_VAL other;
    do {
        other = eval(ptr);
        result.value = result.value * other.value;

        if(other.type == DOUBLE_TYPE)
            result.type = DOUBLE_TYPE;

        ptr = ptr->next;
    }while(ptr != NULL);

    return result;
}

RET_VAL evalDivFunction(AST_NODE *node)
{
    //if it has no operand or only one arg
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if more than 2 operand

    AST_NODE *currentVal = node->data.function.opList;
    RET_VAL result = ZERO_RET_VAL;
    RET_VAL otherVal = eval(currentVal->next);
    RET_VAL val = eval(currentVal);

    result.value = val.value / otherVal.value;
    if(val.type == INT_TYPE && otherVal.type == INT_TYPE)
        result.value = trunc(result.value);
    result.type = val.type || otherVal.type;

    return result;
}

RET_VAL evalRemainderFunction(AST_NODE *node)
{
    //if it has no operand or only one arg
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if more than 2 operand

    AST_NODE *currentVal = node->data.function.opList;
    RET_VAL result = ZERO_RET_VAL;
    RET_VAL otherVal = eval(currentVal->next);
    RET_VAL val = eval(currentVal);
    result.value = fmod(val.value, otherVal.value);
    if(result.value < 0)
        result.value += fabs(otherVal.value);
    result.type = val.type || otherVal.type;

    return result;
}

RET_VAL evalExpFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if more than 1 operand
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);
    result.type = DOUBLE_TYPE;
    result.value = exp(result.value);

    return result;
}

RET_VAL evalExp2Function(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if more than 1 operand
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);

    if(result.value < 0)
        result.type = DOUBLE_TYPE;

    result.value = exp2(result.value);

    return result;
}

RET_VAL evalPowFunction(AST_NODE *node)
{
    //if no operand or only one operand
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if more than 2 operands

    AST_NODE *currentVal = node->data.function.opList;
    RET_VAL result = ONE_RET_VAL;
    RET_VAL otherVal = eval(currentVal->next);
    RET_VAL val = eval(currentVal);
    result.value = pow(val.value,otherVal.value);
    result.type = val.type || otherVal.type;

    return result;
}

RET_VAL evalLogFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if more than 2 operand
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);
    result.value = log(result.value);
    result.type = DOUBLE_TYPE;

    return result;
}

RET_VAL evalSqrtFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if only one operand
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);
    result.value = sqrt(result.value);
    result.type = DOUBLE_TYPE;

    return result;
}

RET_VAL evalCbrtFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }
    //if only one operand
    hasExtraOperands(node);

    RET_VAL result = eval(node->data.function.opList);
    result.value = cbrt(result.value);
    result.type = DOUBLE_TYPE;

    return result;
}

RET_VAL evalHypotFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return ZERO_RET_VAL;
    }

    RET_VAL result = ZERO_RET_VAL;
    AST_NODE *ptr = node->data.function.opList;
    RET_VAL other;
    do {
        other = eval(ptr);
        result.value = hypot(result.value, other.value);
        ptr = ptr->next;
    }while(ptr != NULL);

    result.type = DOUBLE_TYPE;

    return result;
}
RET_VAL evalMaxFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }

    AST_NODE *ptr = node->data.function.opList;
    RET_VAL result = eval(ptr);
    RET_VAL other;
    do {
        other = eval(ptr);
        if(result.value < other.value)
            result = other;

        ptr = ptr->next;
    }while(ptr != NULL);

    return result;
}
RET_VAL evalMinFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }

    AST_NODE *ptr = node->data.function.opList;
    RET_VAL result = eval(ptr);
    RET_VAL other;
    do {
        other = eval(ptr);
        if(result.value > other.value)
            result = other;

        ptr = ptr->next;
    }while(ptr != NULL);

    return result;
}

RET_VAL evalRandFunction()
{
    return (RET_VAL){DOUBLE_TYPE,(double) rand()/RAND_MAX};
}
RET_VAL evalReadFunction()
{
    RET_VAL result = NAN_RET_VAL;
    char *input = calloc(sizeof(char), 32);
    printf("read :: ");
    fscanf(read_target,"%s",input);

    if(read_target->_file)
        printf("%s\n",input);

    //pre-check to ensure we don't have to loop
    //Checked if the first char is a number,
    // not a decimal, or neither a '+' or '-'

    if((input[0] != '+' && input[0] != '-') && (!isdigit(input[0]) || input[0] == '.'))
    {
        warning("Invalid read entry! NAN returned!");
        free(input);
        return NAN_RET_VAL;
    }

    if(isdigit(input[0]))
        result.type = INT_TYPE;

    bool isDouble = false;
    int eachChar = 1;
    while (isdigit(input[eachChar]) || input[eachChar] == '.')
    {
        if(!isDouble)
            result.type = INT_TYPE;

        if(input[eachChar] == '.')
        {
            if(!isDouble)
            {
                result.type = DOUBLE_TYPE;
                isDouble = true;

            }else{
                warning("Invalid read entry! NAN returned!");
                return NAN_RET_VAL;
            }
        }

        eachChar++;
    }

    if(eachChar < strlen(input))
    {
        warning("Invalid read entry! NAN returned!");
        free(input);
        return NAN_RET_VAL;
    }

    result.value = strtod(input,NULL);
    free(input);

    return result;
}

RET_VAL evalPrintFunction(AST_NODE *node)
{
    if (!hasOperand(node))
    {
        return NAN_RET_VAL;
    }

    hasExtraOperands(node);//if more than 2 operand

    RET_VAL result = eval(node->data.function.opList);
    printRetVal(result);

    return result;
}

RET_VAL evalEqualFunction(AST_NODE *node)
{
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    AST_NODE *currentVal = node->data.function.opList;

    //Modified
    return (eval(currentVal).value == eval(currentVal->next).value) ? ONE_RET_VAL : ZERO_RET_VAL;
}

RET_VAL evalLesserFunction(AST_NODE *node)
{
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    AST_NODE *currentVal = node->data.function.opList;

    //Modified
    return (eval(currentVal).value < eval(currentVal->next).value) ? ONE_RET_VAL : ZERO_RET_VAL;
}

RET_VAL evalGreaterFunction(AST_NODE *node)
{
    if (!hasOperand(node) || hasOnlyOneOperand(node))
    {
        return NAN_RET_VAL;
    }

    AST_NODE *currentVal = node->data.function.opList;

    //Modified
    return (eval(currentVal).value > eval(currentVal->next).value) ? ONE_RET_VAL : ZERO_RET_VAL;
}

RET_VAL evalFuncNode(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into evalFuncNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    // TODO complete the function
    // HINT:
    // the helper functions that it calls will need to be defined above it
    // because they are not declared in the .h file (and should not be)

    switch (node->data.function.func)
    {
        case NEG_FUNC:
            return evalNegFunction(node);
        case ABS_FUNC:
            return evalAbsFunction(node);
        case ADD_FUNC:
            return evalAddFunction(node);//n-ary
        case SUB_FUNC:
            return evalSubFunction(node);//binary
        case MULT_FUNC:
            return evalMultFunction(node);//n-ary
        case DIV_FUNC:
            return evalDivFunction(node);//binary
        case REMAINDER_FUNC:
            return evalRemainderFunction(node);
        case EXP_FUNC:
            return evalExpFunction(node);
        case EXP2_FUNC:
            return evalExp2Function(node);
        case POW_FUNC:
            return evalPowFunction(node);
        case LOG_FUNC:
            return evalLogFunction(node);
        case SQRT_FUNC:
            return evalSqrtFunction(node);
        case CBRT_FUNC:
            return evalCbrtFunction(node);
        case HYPOT_FUNC:
            return evalHypotFunction(node);//n-ary
        case MAX_FUNC:
            return evalMaxFunction(node);//n-ary
        case MIN_FUNC:
            return evalMinFunction(node);//n-ary
        case RAND_FUNC:
            return evalRandFunction();
        case READ_FUNC:
            return evalReadFunction();
        case PRINT_FUNC:
            return evalPrintFunction(node);
        case EQUAL_FUNC:
            return evalEqualFunction(node);//binary
        case LESS_FUNC:
            return evalLesserFunction(node);//binary
        case GREATER_FUNC:
            return evalGreaterFunction(node);//binary
         case CUSTOM_FUNC:
             return evalCustomFunction(node);
    }
    return NAN_RET_VAL;
}

RET_VAL evalNumNode(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into evalNumNode!");
        return NAN_RET_VAL;
    }

    // TODO complete the function
    return node->data.number;
}

RET_VAL eval(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into eval!");
        return NAN_RET_VAL;
    }
    // TODO complete the function
    switch (node->type)
    {
        case NUM_NODE_TYPE:
            return evalNumNode(node);
        case FUNC_NODE_TYPE:
            return evalFuncNode(node);
        case SYM_NODE_TYPE:
            return evalSymbolFunction(node);
        case SCOPE_NODE_TYPE:
            return eval(node->data.scope.child);
        case COND_NODE_TYPE:
            return evalCondFunction(node);
        default:
            return NAN_RET_VAL;
    }
}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    switch (val.type)
    {
        case INT_TYPE:
            printf("Integer : %.lf\n", val.value);
            break;
        case DOUBLE_TYPE:
            printf("Double : %lf\n", val.value);
            break;
        default:
            printf("No Type : %lf\n", val.value);
            break;
    }
}

void freeFunctionNode(AST_NODE *node)
{
    if(node == NULL)
    {
        return;
    }
    free(node->data.function.id);
    freeNode(node->data.function.opList);
}

void freeStackNode(STACK_NODE *node)
{
    if(!node)
        return;

    freeStackNode(node->next);
    free(node);
}

void freeSymbolTableNode(SYMBOL_TABLE_NODE *node)
{
    if(!node)
        return;

    freeSymbolTableNode(node->next);
    freeStackNode(node->stack);
    freeNode(node->value);
    free(node->id);
    free(node);
}


void freeNode(AST_NODE *node)
{
    if (!node)
    {
        return;
    }

    // TODO complete the function

    // look through the AST_NODE struct, decide what
    // referenced data should have freeNode called on it
    // (hint: it might be part of an s_expr_list, with more
    // nodes following it in the list)

    // if this node is FUNC_TYPE, it might have some operands
    // to free as well (but this should probably be done in
    // a call to another function, named something like
    // freeFunctionNode)

    freeNode(node->next);

    switch (node->type)
    {
        case NUM_NODE_TYPE:
            break;
        case SYM_NODE_TYPE:
            free(node->data.symbol.id);
            break;
        case FUNC_NODE_TYPE:
        case COND_NODE_TYPE:
            freeFunctionNode(node);
            break;
        case SCOPE_NODE_TYPE:
            freeNode(node->data.scope.child);
            break;
    }

    freeSymbolTableNode(node->symbolTable);
    freeSymbolTableNode(node->argTable);
    free(node);
}
//String literals, string operations
//type cast to arg (whether good or bad) -> print warning if incorrect
//boolean literals, true or false
//mimic of python or java -> more functional language

//Study up: SQL lang, PYTHON!!!!
//NUMPY,list comprehension