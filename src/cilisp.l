%option noyywrap
%option noinput
%option nounput

%{
    #include "cilisp.h"
    #define llog(token) {fprintf(flex_bison_log_file, "LEX: %s \"%s\"\n", #token, yytext); fflush(stdout);}
%}

digit       [0-9]
letter      [a-z]|[A-Z]|"_"|"$"
int         [+-]?{digit}+
double      [+-]?{digit}+\.{digit}*
symbol      {letter}({digit}|{letter})*
lparen      "("
rparen      ")"
func        "neg"|"abs"|"add"|"sub"|"mult"|"div"|"remainder"|"exp"|"exp2"|"pow"|"log"|"sqrt"|"cbrt"|"hypot"|"max"|"min"|"rand"|"read"|"print"|"greater"|"less"|"equal"|"custom"
let         "let"
type        "int"|"double"
cond        "cond"
lambda      "lambda"
quit        "quit"

%%

{int} {
    llog(INT);
    yylval.dval = strtod(yytext, NULL);
    return INT;
}

{double} {
    llog(DOUBLE);
    yylval.dval = strtod(yytext, NULL);
    return DOUBLE;
}

{let} {
     llog(LET);
     return LET;
}

{func} {
    llog(FUNC);
    yylval.ival = resolveFunc(yytext);
    return FUNC;
}

{lparen} {
    llog(LPAREN);
    return LPAREN;
}

{rparen} {
    llog(RPAREN);
    return RPAREN;
}

[\n] {
    llog(EOL);
    return EOL;
    }

[\xff] {
    llog(EOFT);
    return EOFT;
    }

{quit} {
    llog(QUIT);
    return QUIT;
}

{type} {
    llog(type);
    yylval.tval = resolveType(yytext);
    return TYPE;
}

{cond} {
    llog(cond);
    return COND;
}

{lambda} {
    llog(lambda);
    return LAMBDA;
}

{symbol} {
    llog(SYMBOL);
    yylval.sval = calloc(sizeof(char),strlen(yytext) + 1);
    strcpy(yylval.sval,yytext);
    return SYMBOL;
}

[ \t\r] ; /* skip whitespace */

. { // anything else
    llog(INVALID);
    warning("Invalid character >>%s<<", yytext);
    }

%%

// Edit at your own risk.

#include <stdio.h>
#include "cilisp.h"
#include "yyreadprint.c"

int main(int argc, char **argv)
{
    flex_bison_log_file = fopen(BISON_FLEX_LOG_PATH, "w");

    if (argc > 2) read_target = fopen(argv[2], "r");
    else read_target = stdin;

    bool input_from_file;
    if ((input_from_file = argc > 1))
    {
        stdin = fopen(argv[1], "r");
    }

    char *s_expr_str = NULL;
    size_t s_expr_str_len = 0;
    size_t s_expr_postfix_padding = 2;
    YY_BUFFER_STATE buffer;

    while (true)
    {
        printf("\n> ");

        s_expr_str = NULL;
        s_expr_str_len = 0;
        yyreadline(&s_expr_str, &s_expr_str_len, stdin, s_expr_postfix_padding);

        while (s_expr_str[0] == '\n')
        {
            yyreadline(&s_expr_str, &s_expr_str_len, stdin, s_expr_postfix_padding);
        }

        if (input_from_file)
        {
            yyprintline(s_expr_str, s_expr_str_len, s_expr_postfix_padding);
        }

        buffer = yy_scan_buffer(s_expr_str, s_expr_str_len);

        yyparse();

        yy_flush_buffer(buffer);
        yy_delete_buffer(buffer);
        free(s_expr_str);
    }
}