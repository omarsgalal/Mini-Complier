typedef enum { typeCon, typeId, typeOpr } nodeEnum;
typedef enum { intState, floatState, charState, noneState } stateEnum;

#include <map> 
#include <utility>  
#include <string.h>  
using namespace std;
/* constants */
typedef struct {
    int type;
    int constant;
    int initialized;
    union {
        int value;                  /* value of constant */
        float valueF;
        char valueC;
    };
    
} conNodeType;

typedef struct {
    string sIndex;                /* symbol table index */
    int scope;
} VarInfo;



/* identifiers */
typedef struct {
    string i;                      /* subscript to sym array */
    int scope;
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

extern map<int, map<string ,conNodeType*>> sym;
