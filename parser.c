#include <stdio.h>
#include "includes.h"
#include "y.tab.h"

void yyerror(char *s);

static FILE *quadsFile = fopen("quadruples.txt", "w");
static int lbl;

void* printQuads(nodeType *p) {
    int lbl1, lbl2;
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       fprintf(quadsFile, "push\t\"%d\"\n", p->con.value);   return 0;
            case FLOAT:         fprintf(quadsFile, "push\t\"%f\"\n", p->con.valueF);  return 0;
            case CHARACTER:     fprintf(quadsFile, "push\t\"%c\"\n", p->con.valueC);  return 0;
            case STRING:        fprintf(quadsFile, "push\t\"%s\"\n", p->con.valueS);  return 0;
        }     
    
    case typeId:        fprintf(quadsFile, "push\t%s\n", p->id.i);     return 0;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     
            // while(pri(p->opr.op[0])) pri(p->opr.op[1]); return 0;
            fprintf(quadsFile, "L%03d:\n", lbl1 = lbl++);
            pri(p->opr.op[0]);
            fprintf(quadsFile, "jtrue\tL%03d\n", lbl2 = lbl++);
            pri(p->opr.op[1]);
            fprintf(quadsFile, "jmp\tL%03d\n", lbl1);
            fprintf(quadsFile, "L%03d:\n", lbl2);
            break;

        case DOWHILE:   
            // do {pri(p->opr.op[1]);} while(pri(p->opr.op[0])); return 0;
            fprintf(quadsFile, "L%03d:\n", lbl1 = lbl++);
            pri(p->opr.op[1]);
            pri(p->opr.op[0]);
            fprintf(quadsFile, "jtrue\tL%03d\n", lbl2 = lbl++);
            fprintf(quadsFile, "jmp\tL%03d\n", lbl1);
            fprintf(quadsFile, "L%03d:\n", lbl2);
            break;

        case FOR:       
            // for(ex(p->opr.op[0]); ex(p->opr.op[1]); ex(p->opr.op[2])) ex(p->opr.op[3]); return 0;
            pri(p->opr.op[0]);
            fprintf(quadsFile, "L%03d:\n", lbl1 = lbl++);
            pri(p->opr.op[1]);
            fprintf(quadsFile, "jtrue\tL%03d\n", lbl2 = lbl++);
            pri(p->opr.op[3]);
            pri(p->opr.op[2]);
            fprintf(quadsFile, "jmp\tL%03d\n", lbl1);
            fprintf(quadsFile, "L%03d:\n", lbl2);
            break;
        case IF:        
            // if (ex(p->opr.op[0]))
            //     ex(p->opr.op[1]);
            // else if (p->opr.nops > 2)
            //     ex(p->opr.op[2]);
            // return 0;
            if (p->opr.nops > 2)
            {
                pri(p->opr.op[0]);
                fprintf(quadsFile, "jtrue\tL%03d\n", lbl2 = lbl++);
                pri(p->opr.op[1]);
                fprintf(quadsFile, "jmp\tL%03d\n", lbl1 = lbl++);
                fprintf(quadsFile, "L%03d:\n", lbl2);
                pri(p->opr.op[2]);
                fprintf(quadsFile, "L%03d:\n", lbl1);
            } else {
                pri(p->opr.op[0]);
                fprintf(quadsFile, "jtrue\tL%03d\n", lbl2 = lbl++);
                pri(p->opr.op[1]);
                fprintf(quadsFile, "L%03d:\n", lbl2);
            }
            break;

        case PRINT:     pri(p->opr.op[0]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\nprint,\ts1,\t  ,\t\n", p->id.i); return 0;
        case ';':       pri(p->opr.op[0]); return pri(p->opr.op[1]);
        case '=':       pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\nmov,\ts1,\t  ,\t%s\n", p->opr.op[0]->id.i); return 0;
        case UMINUS:    pri(p->opr.op[0]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\nneg,\ts1,\t  ,\ts3\npush\ts3\n"); return 0;
        case '!':       pri(p->opr.op[0]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\nnot,\ts1,\t  ,\ts3\npush\ts3\n"); return 0;
        case '+':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\nadd,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '-':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\nsub,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '*':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\nmul,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '/':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ndiv,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '%':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\nmod,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '<':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompLT,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case '>':       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompGT,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case AND:       pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompAND,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case OR:        pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompOR,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case GE:        pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompGE,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case LE:        pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompLE,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case NE:        pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompNE,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        case EQ:        pri(p->opr.op[0]); pri(p->opr.op[1]); fprintf(quadsFile, "pop,\t  ,\t  ,\ts1\npop,\t  ,\t  ,\ts2\ncompEQ,\ts1,\ts2,\ts3\npush\ts3\n"); return 0;
        }
    }
    return 0;
}

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       return p->con.value;
            case FLOAT:         return p->con.valueF;
            case CHARACTER:     return p->con.valueC;
            //case STRING:        return p->con.valueS;
        }     
    
    case typeId:        return sym[p->id.i]->value;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        case DOWHILE:   do {ex(p->opr.op[1]);} while(ex(p->opr.op[0])); return 0;
        case FOR:       for(ex(p->opr.op[0]); ex(p->opr.op[1]); ex(p->opr.op[2])) ex(p->opr.op[3]); return 0;
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       sym[p->opr.op[0]->id.i]->initialized = 1; sym[p->opr.op[0]->id.i]->value = ex(p->opr.op[1]); printSymbolTable(); return 1;
        case UMINUS:    return -ex(p->opr.op[0]);
        case '+':       return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case '%':       return ex(p->opr.op[0]) % ex(p->opr.op[1]);
        case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case AND:       return ex(p->opr.op[0]) && ex(p->opr.op[1]);
        case OR:        return ex(p->opr.op[0]) || ex(p->opr.op[1]);
        case '!':       return !ex(p->opr.op[0]);
        case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        }
    }
    return 0;
}

float exF(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       return p->con.value;
            case FLOAT:         return p->con.valueF;
            case CHARACTER:     return p->con.valueC;
            //case STRING:        return p->con.valueS;
        }     
    
    case typeId:        return sym[p->id.i]->valueF;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(exF(p->opr.op[0])) exF(p->opr.op[1]); return 0;
        case DOWHILE:   do {exF(p->opr.op[1]);} while(exF(p->opr.op[0])); return 0;
        case FOR:       for(exF(p->opr.op[0]); exF(p->opr.op[1]); exF(p->opr.op[2])) exF(p->opr.op[3]); return 0;
        case IF:        if (exF(p->opr.op[0]))
                            exF(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            exF(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%f\n", exF(p->opr.op[0])); return 0;
        case ';':       exF(p->opr.op[0]); return exF(p->opr.op[1]);
        case '=':       sym[p->opr.op[0]->id.i]->initialized = 1; sym[p->opr.op[0]->id.i]->valueF = exF(p->opr.op[1]); printSymbolTable(); return 1;
        case UMINUS:    return -exF(p->opr.op[0]);
        case '+':       return exF(p->opr.op[0]) + exF(p->opr.op[1]);
        case '-':       return exF(p->opr.op[0]) - exF(p->opr.op[1]);
        case '*':       return exF(p->opr.op[0]) * exF(p->opr.op[1]);
        case '/':       return exF(p->opr.op[0]) / exF(p->opr.op[1]);
        case '<':       return exF(p->opr.op[0]) < exF(p->opr.op[1]);
        case '>':       return exF(p->opr.op[0]) > exF(p->opr.op[1]);
        case '%':       yyerror("operation not defined"); return 0;
        case AND:       return exF(p->opr.op[0]) && exF(p->opr.op[1]);
        case OR:        return exF(p->opr.op[0]) || exF(p->opr.op[1]);
        case '!':       return !exF(p->opr.op[0]);
        case GE:        return exF(p->opr.op[0]) >= exF(p->opr.op[1]);
        case LE:        return exF(p->opr.op[0]) <= exF(p->opr.op[1]);
        case NE:        return exF(p->opr.op[0]) != exF(p->opr.op[1]);
        case EQ:        return exF(p->opr.op[0]) == exF(p->opr.op[1]);
        }
    }
    return 0;
}

char exC(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       return p->con.value;
            case FLOAT:         return p->con.valueF;
            case CHARACTER:     return p->con.valueC;
            //case STRING:        return p->con.valueS;
        }     
    
    case typeId:        return sym[p->id.i]->valueC;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(exC(p->opr.op[0])) exC(p->opr.op[1]); return 0;
        case DOWHILE:   do {exC(p->opr.op[1]);} while(exC(p->opr.op[0])); return 0;
        case FOR:       for(exC(p->opr.op[0]); exC(p->opr.op[1]); exC(p->opr.op[2])) exC(p->opr.op[3]); return 0;
        case IF:        if (exC(p->opr.op[0]))
                            exC(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            exC(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%c\n", exC(p->opr.op[0])); return 0;
        case ';':       exC(p->opr.op[0]); return exC(p->opr.op[1]);
        case '=':       sym[p->opr.op[0]->id.i]->initialized = 1; sym[p->opr.op[0]->id.i]->valueC = exC(p->opr.op[1]); printSymbolTable(); return 1;
        case UMINUS:    return -exC(p->opr.op[0]);
        case '+':       return exC(p->opr.op[0]) + exC(p->opr.op[1]);
        case '-':       return exC(p->opr.op[0]) - exC(p->opr.op[1]);
        case '*':       return exC(p->opr.op[0]) * exC(p->opr.op[1]);
        case '/':       return exC(p->opr.op[0]) / exC(p->opr.op[1]);
        case '%':       return exC(p->opr.op[0]) % exC(p->opr.op[1]);
        case '<':       return exC(p->opr.op[0]) < exC(p->opr.op[1]);
        case '>':       return exC(p->opr.op[0]) > exC(p->opr.op[1]);
        case AND:       return exC(p->opr.op[0]) && exC(p->opr.op[1]);
        case OR:        return exC(p->opr.op[0]) || exC(p->opr.op[1]);
        case '!':       return !exC(p->opr.op[0]);
        case GE:        return exC(p->opr.op[0]) >= exC(p->opr.op[1]);
        case LE:        return exC(p->opr.op[0]) <= exC(p->opr.op[1]);
        case NE:        return exC(p->opr.op[0]) != exC(p->opr.op[1]);
        case EQ:        return exC(p->opr.op[0]) == exC(p->opr.op[1]);
        }
    }
    return 0;
}

char* exS(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            /*case INTEGER:       return p->con.value;
            case FLOAT:         return p->con.valueF;
            case CHARACTER:     return p->con.valueC;*/
            case STRING:        return p->con.valueS;
        }     
    
    case typeId:        return sym[p->id.i]->valueS;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0])) exS(p->opr.op[1]); return 0;
        case DOWHILE:   do {exS(p->opr.op[1]);} while(ex(p->opr.op[0])); return 0;
        case FOR:       for(ex(p->opr.op[0]); ex(p->opr.op[1]); ex(p->opr.op[2])) exS(p->opr.op[3]); return 0;
        case IF:        if (ex(p->opr.op[0]))
                            exS(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            exS(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%s\n", exS(p->opr.op[0])); return 0;
        case ';':       exS(p->opr.op[0]); return exS(p->opr.op[1]);
        case '=':       sym[p->opr.op[0]->id.i]->initialized = 1; sym[p->opr.op[0]->id.i]->valueS = exS(p->opr.op[1]); printSymbolTable(); return 0;
        case '+':       {string ret = string(exS(p->opr.op[0])) + exS(p->opr.op[1]); char* cret = new char[ret.size()+1]; strcpy(cret, ret.c_str()); return cret;}
        case '-':       yyerror("operation not defined"); return "e";
        case '*':       yyerror("operation not defined"); return "e";
        case '/':       yyerror("operation not defined"); return "e";
        case '%':       yyerror("operation not defined"); return "e";
        case '<':       yyerror("operation not defined"); return "e";
        case '>':       yyerror("operation not defined"); return "e";
        case AND:       yyerror("operation not defined"); return "e";
        case OR:        yyerror("operation not defined"); return "e";
        case '!':       yyerror("operation not defined"); return "e";
        case GE:        yyerror("operation not defined"); return "e";
        case LE:        yyerror("operation not defined"); return "e";
        case NE:        yyerror("operation not defined"); return "e";
        case EQ:        yyerror("operation not defined"); return "e";
        }
    }
    return 0;
}