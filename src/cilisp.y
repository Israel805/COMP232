%{
    #include "cilisp.h"
    #define ylog(r, p) {fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p); fflush(stdout);}
    int yylex();
    void yyerror(char*,...);
%}

%union {
    double dval;
    int ival;
    char* sval;
    char *tval;
    struct ast_node *astNode;
    struct symbol_table_node *stNode;
};

%token <ival> FUNC
%token <dval> INT DOUBLE
%token <sval> SYMBOL
%token <tval> TYPE
%token QUIT LET LAMBDA COND LPAREN RPAREN EOL EOFT

%type <astNode> s_expr f_expr number s_expr_section s_expr_list
%type <stNode> let_section let_elem let_list arg_list
%%

program:
    s_expr EOL {
        ylog(program, s_expr EOL);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
        YYACCEPT;
    }
    | s_expr EOFT {
        ylog(program, s_expr EOFT);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
        exit(EXIT_SUCCESS);
    }
    | EOL {
        ylog(program, EOL);
        YYACCEPT;  // paranoic; main skips blank lines
    }
    | EOFT {
        ylog(program, EOFT);
        exit(EXIT_SUCCESS);
    };


s_expr:
    f_expr {
        ylog(s_expr,f_expr);
        $$ = $1;
    }
    | number{
        ylog(s_expr,number);
        $$ = $1;
    }
    | SYMBOL {
        ylog(s_expr,SYMBOL);
        $$ = createSymbolNode($1);
    }
    | LPAREN let_section s_expr RPAREN {
          ylog(s_expr,LPAREN let_section s_expr RPAREN);
          $$ = createScopeNode($2,$3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
        ylog(s_expr, LPAREN COND s_expr s_expr s_expr RPAREN);
        $$ = createCondNode($3,$4,$5);
    }
    | QUIT {
        ylog(s_expr, QUIT);
        exit(EXIT_SUCCESS);
    }
    | error {
        ylog(s_expr, error);
        yyerror("unexpected token");
        $$ = NULL;
    };

let_section:
    LPAREN LET let_list RPAREN {
        ylog(let_elem,LPAREN let_list RPAREN);
        $$ = $3;
    };

let_list:
    let_elem {
        ylog(let_list,let_elem);
        $$ = $1;
    }
    | let_elem let_list {
        ylog(let_list,let_elem let_list);
        $$ = addSymbolToList($1,$2);
    };

let_elem:
    LPAREN SYMBOL s_expr RPAREN {
        ylog(let_elem,LPAREN SYMBOL s_expr RPAREN);
        $$ = createSymbolTableNode(NO_TYPE,VAR_TYPE,$2,NULL,$3);
    }
    | LPAREN TYPE SYMBOL s_expr RPAREN {
        ylog(let_elem,LPAREN TYPE SYMBOL s_expr RPAREN);
        $$ = createSymbolTableNode($2,VAR_TYPE,$3,NULL,$4);
    }
    | LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        ylog(let_elem,LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN);
        $$ = createSymbolTableNode(NO_TYPE,LAMBDA_TYPE,$2,$5,$7);
    }
    | LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        ylog(let_elem,LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN);
        $$ = createSymbolTableNode($2,LAMBDA_TYPE,$3,$6,$8);
    };

arg_list:
    SYMBOL {
        ylog(arg_list,SYMBOL);
        $$ = createArgNode($1);
    }
    | SYMBOL arg_list {
        ylog(arg_list,SYMBOL arg_list);
        $$ = addArgToList($1,$2);
    }
    | {
        ylog(s_expr_section,empty);
        $$ = NULL;
    };

f_expr:
    LPAREN FUNC s_expr_section RPAREN {
        ylog(f_expr, LPAREN FUNC s_expr_section RPAREN);
        $$ = createFunctionNode($2,NULL,$3);
    }
    | LPAREN SYMBOL s_expr_section RPAREN {
        ylog(f_expr, LPAREN SYMBOL s_expr_section RPAREN);
        $$ = createFunctionNode(CUSTOM_FUNC,$2,$3);
    }
    | error {
        ylog(s_expr, error);
        yyerror("unexpected token");
        $$ = NULL;
    };

s_expr_section:
    s_expr_list{
        ylog(s_expr_section,s_expr_list);
        $$ = $1;
    }
    | {
        ylog(s_expr_section,empty);
        $$ = NULL;
    };

s_expr_list:
    s_expr {
        ylog(s_expr_list,s_expr);
        $$ = $1;
    }
    | s_expr s_expr_list {
        ylog(s_expr_list,s_expr s_expr_list);
        $$ = addExpressionToList($1,$2);
    };

number:
    INT{
        ylog(number,INT);
        $$ = createNumberNode($1,INT_TYPE);
    }
    | DOUBLE{
        ylog(number,DOUBLE);
        $$ = createNumberNode($1,DOUBLE_TYPE);
    };

%%

