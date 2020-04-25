%{
#include <stdlib.h>
#include "includes.h"
#include "y.tab.h"
#include "utils.h"
void yyerror(char *);
%}

%%

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]*\.[0-9]+ {
                yylval.fValue = atof(yytext);
                return FLOAT;
            }

0\.[0-9]+    {
                yylval.fValue = atof(yytext);
                return FLOAT;
            }

'[a-z]'     { 
                yylval.cValue = yytext[1];
                return CHARACTER;
            }

[-()<>=+*/%;{}.!] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"for"           return FOR;
"dowhile"       return DOWHILE;
"int"           return INTIDENTIFIER;
"float"         return FLOATIDENTIFIER;
"char"          return CHARIDENTIFIER;
"const"         return CONSTANT;
"&&"            return AND;
"||"            return OR;


[a-zA-Z_][a-zA-Z_0-9]* { 
                yylval.sIndex = strdup(yytext);
                return VARIABLE;
            }

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
