%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "includes.h"
#include <map> 
#include <utility>  
#include <string.h>  
using namespace std;

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(VarInfo i);
nodeType *Defid(VarInfo v);
nodeType *con(int value);
nodeType *conF(float value);
nodeType *conC(char value);
void freeNode(nodeType *p);
void genExecute(nodeType *p);
int ex(nodeType *p);
float exF(nodeType *p);
char exC(nodeType *p);
int yylex(void);
extern FILE *yyin;

void yyerror(char *s);
map<int, map<string ,conNodeType*>> sym;                     /* symbol table */
int status = noneState;
int declareState = noneState;
int ConstOrNot = 0;

%}

%union {
    int iValue;                 /* integer value */
    float fValue;               /* float value */
    char cValue;                /* char value */
    VarInfo var;
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <fValue> FLOAT
%token <cValue> CHARACTER
%token <var> VARIABLE
%token WHILE IF PRINT FOR DOWHILE INTIDENTIFIER CONSTANT FLOATIDENTIFIER CHARIDENTIFIER
%nonassoc IFX
%nonassoc ELSE

%left AND OR '!'
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list assign_stmt declare_stmt declare_assign_stmt const_stmt d_assign_stmt

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { genExecute($2); freeNode($2); }
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
        | FOR '(' declare_assign_stmt expr ';' assign_stmt ')' stmt { $$ = opr(FOR, 4, $3, $4, $6, $8); }
        | IF '(' expr ')' stmt %prec IFX                            { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt                            { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'                                         { $$ = $2; }
        | declare_stmt                                              { $$ = $1; }
        | declare_assign_stmt                                       { $$ = $1; }
        | const_stmt                                                { $$ = $1; }
        ;

assign_stmt:
          VARIABLE '=' expr                         { $$ = opr('=', 2, id($1), $3); }
        ;


d_assign_stmt:
          VARIABLE '=' expr                         { $$ = opr('=', 2, Defid($1), $3); ConstOrNot = 0 ; }
        ;

declare_stmt:
          INTIDENTIFIER VARIABLE ';'                { declareState = intState;   Defid($2);  ConstOrNot = 0 ;}
        | FLOATIDENTIFIER VARIABLE ';'              { declareState = floatState; Defid($2);  ConstOrNot = 0 ;}
        | CHARIDENTIFIER VARIABLE ';'               { declareState = charState;  Defid($2);  ConstOrNot = 0 ;}
        ;

declare_assign_stmt:
          INTIDENTIFIER d_assign_stmt ';'             { $$ = $2; declareState = intState;  }
        | FLOATIDENTIFIER d_assign_stmt ';'           { $$ = $2; declareState = floatState;  }
        | CHARIDENTIFIER d_assign_stmt ';'            { $$ = $2; declareState = charState;  }
        ;

const_stmt:
          CONSTANT declare_stmt                     { $$ = $2; ConstOrNot = 1; }
        | CONSTANT declare_assign_stmt              { $$ = $2; ConstOrNot = 1; }

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = con($1); }
        | FLOAT                 { $$ = conF($1); }
        | CHARACTER             { $$ = conC($1); }
        | VARIABLE              { $$ = id($1); }
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
    if (status == noneState)
        status = intState;

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = INTEGER;
    p->con.value = value;

    return p;
}

nodeType *conF(float value) {
    nodeType *p;
    status = floatState;

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = FLOAT;
    p->con.valueF = value;
    return p;
}

nodeType *conC(char value) {
    nodeType *p;
    status = charState;

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = CHARACTER;
    p->con.valueC = value;

    return p;
}

/* already defined variable*/

nodeType *id(VarInfo v ) {
    nodeType *p;
    int myScope = -1;


    for(int i = v.scope ; i >-1 ; i--){
        if(sym.find(i) != sym.end()){
        if(sym[i].find(v.sIndex) != sym[i].end()){
            myScope = i;
            break;
            }
        }
    }

    if(myScope == -1){
        yyerror("variable used before declared");
    }

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = v.sIndex;
    p->id.scope = myScope;
    return p;
}


/*define variable*/

nodeType *Defid(VarInfo v) {
    nodeType *p;
    conNodeType* dummy;
    if ((dummy = (conNodeType*) malloc(sizeof(conNodeType))) == NULL)
        yyerror("out of memory");
    
    dummy->constant = ConstOrNot;
    dummy->initialized = 0 ;
    if(declareState == intState){
        dummy->value = 0;
        dummy->type = INTEGER;
    }
    else if (declareState == floatState){
        dummy->valueF = 0;
        dummy->type = FLOAT;
    }else{
        dummy->valueC = 'a';
        dummy->type = CHARACTER;
    }
    

    if(sym.find(v.scope) != sym.end()){
        if(sym[v.scope].find(v.sIndex) != sym[v.scope].end()){
              yyerror("dublicate initialization ");
        }else{
             sym[v.scope][v.sIndex]= dummy;
        }

    }else{
        sym[v.scope][v.sIndex]= dummy;
    }
    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = v.sIndex;
    p->id.scope = v.scope;
    return p;
}




nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = (nodeType*) malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
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

void genExecute(nodeType *p) {
    if(status == intState) 
        ex(p); 
    else if (status == floatState) 
        exF(p); 
    else if (status == charState)
        exC(p);
    else
        ex(p); 
    
    status = noneState;
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        yyerror("no file supported!");
        return -1;
    }
        
    FILE *myfile = fopen(argv[1], "r");
    // make sure it is valid:
	if (!myfile) {
		// cout << "I can't open a.snazzle.file!" << endl;
        yyerror("Can't open the file!");
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

    // yyparse();
    return 0;
}
