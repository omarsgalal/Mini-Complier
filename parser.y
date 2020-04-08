%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "includes.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
nodeType *conF(float value);
nodeType *conC(char value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(char *s);
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    float fValue;               /* float value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <fValue> FLOAT
%token <sIndex> VARIABLE
%token WHILE IF PRINT FOR DOWHILE INTIDENTIFIER CONSTANT
%nonassoc IFX
%nonassoc ELSE

%left AND OR '!'
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list assign_stmt declare_stmt declare_assign_stmt

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* NULL */
        ;

stmt:
          ';'                                                       { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                                                  { $$ = $1; }
        | PRINT expr ';'                                            { $$ = opr(PRINT, 1, $2); }
        | assign_stmt ';'                                           { $$ = $1; }
        | WHILE '(' expr ')' stmt                                   { $$ = opr(WHILE, 2, $3, $5); }
        | DOWHILE '(' expr ')' stmt                                 { $$ = opr(DOWHILE, 2, $3, $5); }
        | FOR '(' assign_stmt ';' expr ';' assign_stmt ')' stmt     { $$ = opr(FOR, 4, $3, $5, $7, $9); }
        | FOR '(' declare_assign_stmt expr ';' assign_stmt ')' stmt     { $$ = opr(FOR, 4, $3, $4, $6, $8); }
        | IF '(' expr ')' stmt %prec IFX                            { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt                            { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'                                         { $$ = $2; }
        | declare_stmt                                              { $$ = $1; }
        | declare_assign_stmt                                       { $$ = $1; }
        ;

assign_stmt:

          VARIABLE '=' expr     { $$ = opr('=', 2, id($1), $3); }
        ;

declare_stmt:
          INTIDENTIFIER VARIABLE ';'                { $$ = opr('=', 2, id($2), 0); }
        | CONSTANT INTIDENTIFIER VARIABLE ';'       { $$ = opr('=', 2, id($3), 0); }
        ;

declare_assign_stmt:
          INTIDENTIFIER assign_stmt ';'             { $$ = $2; }
        | CONSTANT INTIDENTIFIER assign_stmt ';'    { $$ = $3; }
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = con($1); }
        | VARIABLE              { $$ = id($1); }
        | FLOAT                 { $$ = conF($1); }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '%' expr         { $$ = opr('%', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr AND expr         { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
        | '!' expr              { $$ = opr('!', 1, $2); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

nodeType *con(int value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = INTEGER;
    p->con.value = value;

    return p;
}

nodeType *conF(float value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = FLOAT;
    p->con.valueF = value;

    return p;
}

nodeType *conC(char value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = INTEGER;
    p->con.valueC = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
