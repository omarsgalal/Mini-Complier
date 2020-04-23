#include <stdio.h>
#include "includes.h"
#include "y.tab.h"

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       return p->con.value;
            case FLOAT:         return p->con.valueF;
            case CHARACTER:     return p->con.valueC;
        }     
    
    case typeId:        return sym[p->id.scope][p->id.i]->value;
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
        case '=':       return sym[p->opr.op[0]->id.scope][p->opr.op[0]->id.i]->value = ex(p->opr.op[1]);
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
        }     
    
    case typeId:        return sym[p->id.scope][p->id.i]->valueF;
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
        case '=':       return sym[p->opr.op[0]->id.scope][p->opr.op[0]->id.i]->valueF = exF(p->opr.op[1]);
        case UMINUS:    return -exF(p->opr.op[0]);
        case '+':       return exF(p->opr.op[0]) + exF(p->opr.op[1]);
        case '-':       return exF(p->opr.op[0]) - exF(p->opr.op[1]);
        case '*':       return exF(p->opr.op[0]) * exF(p->opr.op[1]);
        case '/':       return exF(p->opr.op[0]) / exF(p->opr.op[1]);
        case '<':       return exF(p->opr.op[0]) < exF(p->opr.op[1]);
        case '>':       return exF(p->opr.op[0]) > exF(p->opr.op[1]);
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
        }     
    
    case typeId:        return sym[p->id.scope][p->id.i]->valueC;
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
        case '=':       return sym[p->opr.op[0]->id.scope][p->opr.op[0]->id.i]->valueC = exC(p->opr.op[1]);
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