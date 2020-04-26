%{
#include <stdlib.h>
#include "includes.h"
#include "y.tab.h"
#include "utils.h"
void yyerror(char *);

%}

%option yylineno

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

'[a-zA-Z0-9_()<>=+*/%;{}.!]'     { 
                yylval.cValue = yytext[1];
                return CHARACTER;
            }

\"([^\\\"]|\\.)*\"  {
                        yylval.sValue = strdup(yytext);
                        return STRING;
                    }

[-()<>=+*/%;{}.!] {
                return *yytext;
             }

">="            { yylval.nPtr = NULL; return GE; }
"<="            { yylval.nPtr = NULL; return LE; }
"=="            { yylval.nPtr = NULL; return EQ; }
"!="            { yylval.nPtr = NULL; return NE; }
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"for"           return FOR;
"dowhile"       return DOWHILE;
"int"           { yylval.nPtr = NULL; return INTIDENTIFIER; }
"float"         { yylval.nPtr = NULL; return FLOATIDENTIFIER; }
"char"          { yylval.nPtr = NULL; return CHARIDENTIFIER; }
"string"        { yylval.nPtr = NULL; return STRINGIDENTIFIER; }
"const"         { yylval.nPtr = NULL; return CONSTANT; }
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

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s); 
}

void symbolLine()
{
    symbolTableFile << "Symbol table change at line: " << yylineno << endl << endl;
}