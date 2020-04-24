#include <stdio.h>
#include "includes.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:  
        switch(p->con.type) {
            case INTEGER:       printf("\tpush\t%d\n", p->con.value); break;
            case FLOAT:         printf("\tpush\t%f\n", p->con.valueF); break;
            case CHARACTER:     printf("\tpush\t%c\n", p->con.valueC); break;
        }     
    
    case typeId:        printf("\tpush\t%c\n", p->id.i + 'a');  break;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case DOWHILE:   
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[1]);
            ex(p->opr.op[0]);
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case FOR:       
            // for(ex(p->opr.op[0]); ex(p->opr.op[1]); ex(p->opr.op[2])) ex(p->opr.op[3]); return 0;
            ex(p->opr.op[0]);
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[2]);
            printf("\tjz\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;

        case IF:        
            // if (ex(p->opr.op[0]))
            //     ex(p->opr.op[1]);
            // else if (p->opr.nops > 2)
            //     ex(p->opr.op[2]);
            // return 0;
            if (p->opr.nops > 2)
            {
                ex(p->opr.op[0]);
                printf("\tjz\tL%03d\n", lbl2 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl1 = lbl++);
                printf("L%03d:\n", lbl2);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl1);
            } else {
                ex(p->opr.op[0]);
                printf("\tjz\tL%03d\n", lbl2 = lbl++);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl2);
            }
            break;

        case PRINT:     
            // printf("%d\n", ex(p->opr.op[0])); return 0;
            ex(p->opr.op[0]);
            printf("\tprint\n");
            break;
        case ';':       
            ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       
            // return sym[p->opr.op[0]->id.i]->value = ex(p->opr.op[1]);
            ex(p->opr.op[1]);
            printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
            break;

        case UMINUS:    
            // return -ex(p->opr.op[0]);
            ex(p->opr.op[0]);
            printf("\tneg\n");
            break;
        case '!':       
            // return !ex(p->opr.op[0]);
            ex(p->opr.op[0]);
            printf("\tnot\n");
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
            case '+':    printf("\tadd\n"); break;
            case '-':    printf("\tsub\n"); break;
            case '*':    printf("\tmul\n"); break;
            case '/':    printf("\tdiv\n"); break;
            case '%':    printf("\tmod\n"); break;
            case '<':    printf("\tcompLT\n"); break;
            case '>':    printf("\tcompGT\n"); break;
            case AND:    printf("\tcompAND\n"); break;
            case OR:     printf("\tcompOR\n"); break;
            case GE:     printf("\tcompGE\n"); break;
            case LE:     printf("\tcompLE\n"); break;
            case NE:     printf("\tcompNE\n"); break;
            case EQ:     printf("\tcompEQ\n"); break;
            }
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
        case '=':       return sym[p->opr.op[0]->id.i]->valueF = exF(p->opr.op[1]);
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
        case '=':       return sym[p->opr.op[0]->id.i]->valueC = exC(p->opr.op[1]);
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