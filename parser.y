%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "includes.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* f);
nodeType *con(int value);
nodeType *conF(float value);
nodeType *conC(char value);
nodeType *conS(char* value);
void freeNode(nodeType *p);
void genExecute(nodeType *p);
int ex(nodeType *p);
float exF(nodeType *p);
char exC(nodeType *p);
char* exS(nodeType *p);
int yylex(void);
extern FILE *yyin;
int e = 0;
void yyerror(char *s);
unordered_map<string, conNodeType*> sym;
int status = noneState;
int dontExecute = 0;
int isMismatch = 0;

/*
    0 --> use variable
    1 --> declare
    2 --> assign
*/
int variableState = 0;  
%}

%union {
    int iValue;                 /* integer value */
    float fValue;               /* float value */
    char cValue;                /* char value */
    char* sValue;              /* string value */
    char* sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <fValue> FLOAT
%token <cValue> CHARACTER
%token <sValue> STRING
%token <sIndex> VARIABLE
%token WHILE IF PRINT FOR DOWHILE INTIDENTIFIER CONSTANT FLOATIDENTIFIER CHARIDENTIFIER STRINGIDENTIFIER
%nonassoc IFX
%nonassoc ELSE

%left AND OR '!'
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list assign_stmt declare_stmt declare_assign_stmt const_stmt

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { if(!dontExecute) genExecute($2); freeNode($2); }
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
        | error ';'                                                 { $$ = NULL; dontExecute = 1; yyerrok; yyclearin; }
        | error '}'                                                 { $$ = NULL; dontExecute = 1; yyerrok; yyclearin; }
        ;

assign_stmt:
          VARIABLE '=' expr                         { $$ = opr('=', 2, id($1), $3); }
        ;

declare_stmt:
          INTIDENTIFIER VARIABLE ';'                { variableState = 1; status = intState; $$ = id($2); }
        | FLOATIDENTIFIER VARIABLE ';'              { variableState = 1; status = floatState; $$ = id($2); }
        | CHARIDENTIFIER VARIABLE ';'               { variableState = 1; status = charState; $$ = id($2); }
        | STRINGIDENTIFIER VARIABLE ';'             { variableState = 1; status = stringState; $$ = id($2); }
        ;

declare_assign_stmt:
          INTIDENTIFIER VARIABLE '=' expr ';'         { variableState = 2; status = intState; $$ = opr('=', 2, id($2), $4); }
        | FLOATIDENTIFIER VARIABLE '=' expr ';'       { variableState = 2; status = floatState; $$ = opr('=', 2, id($2), $4); }
        | CHARIDENTIFIER VARIABLE '=' expr ';'        { variableState = 2; status = charState; $$ = opr('=', 2, id($2), $4); }
        | STRINGIDENTIFIER VARIABLE '=' expr ';'      { variableState = 2; status = stringState; $$ = opr('=', 2, id($2), $4); }
        ;

const_stmt:
          CONSTANT declare_stmt                     { $$ = $2; }
        | CONSTANT declare_assign_stmt              { $$ = $2; }

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = con($1); }
        | FLOAT                 { $$ = conF($1); }
        | CHARACTER             { $$ = conC($1); }
        | STRING                { $$ = conS($1); }
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

void typeMismatch(stateEnum currentState)
{
    if (status == noneState || currentState == status)
        status = currentState;
    else
    {
        dontExecute = 1;
        if(isMismatch == 0)
        {
            yyerror("Type mismatch!");
            isMismatch = 1;
        }

    }  
}

nodeType *con(int value) {
    nodeType *p;
    
    typeMismatch(intState);

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

    typeMismatch(floatState);

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

    typeMismatch(charState);

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = CHARACTER;
    p->con.valueC = value;

    return p;
}

nodeType *conS(char* value) {
    nodeType *p;

    typeMismatch(stringState);

    /* allocate node */
    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.type = STRING;
    p->con.valueS = value;

    return p;
}

nodeType *id(char* f) {
    nodeType *p;


    if ((p = (nodeType*) malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    string i = f;

    if(sym.find(i) == sym.end()) 
    {
        if (variableState != 0)
        {
            conNodeType* dummy;
            if ((dummy = (conNodeType*) malloc(sizeof(conNodeType))) == NULL)             
                yyerror("out of memory");                  
                    
            dummy->initialized = 0;   

            if(status == intState)
            {
                dummy->value = 0;             
                dummy->type = INTEGER; 
            }  
            else if (status == floatState)
            {
                dummy->valueF = 0.0;
                dummy->type = FLOAT;
            }
            else if(status == charState)
            {
                dummy->valueC = 'a';
                dummy->type = CHARACTER;
            }    
            else if(status == stringState)
            {
                dummy->valueS = "";
                dummy->type = STRING;
            }    
                    
                    

            sym[i] = dummy;
        }
        else
        {
            yyerror("variable used before declaration!");
            dontExecute = 1;
        }
        
    }
    else
    {
        if (variableState == 1)
            yyerror("variable already declared!");
        
        if (sym[i]->initialized == 0 && variableState == 0)
            yyerror("variable used before initialization! the default value will be used");
            
        if (sym[i]->type == INTEGER)
            typeMismatch(intState);
        else if(sym[i]->type == FLOAT)
            typeMismatch(floatState);
        else if(sym[i]->type == CHARACTER)
            typeMismatch(charState);
        else if(sym[i]->type == STRING)
            typeMismatch(stringState);
    }

    /* copy information */
    p->type = typeId;
    p->id.i = new char[i.size()+1];
    strcpy(p->id.i, i.c_str());

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
    dontExecute = 0;
    isMismatch = 0;
    /*if (e == 1)
    {
        printf("_________freeNode________\n");
        e = 0;
        return;
    }*/
    /*printf("_________freeNode________\n");
    printf("sizeof *p = %d\n", sizeof(*p));
    printf("sizeof nodeType = %d\n", sizeof(nodeType));
    if(sizeof(*p) != sizeof(nodeType))
    {
        printf("________tozbot________\n");
        return;
    }
    if(p == NULL)
        printf("p == NULL\n");
    std::cout<<p<<"\n";*/
    
    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free(p);
}

void genExecute(nodeType *p) {
    /*if (e == 1)
    {
        printf("_________genExecute________\n");
        return;
    }*/
    if(status == intState) 
        ex(p); 
    else if (status == floatState) 
        exF(p); 
    else if (status == charState)
        exC(p);
    else if (status == stringState)
        exS(p);
    else
        ex(p); 
    
    status = noneState;
    variableState = 0;
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
