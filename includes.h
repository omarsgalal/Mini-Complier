#include <map>  
#include <utility>
#include <string.h>

using namespace std;

typedef enum { typeCon, typeId, typeOpr } nodeEnum;
typedef enum { intState, floatState, charState, noneState } stateEnum;

/* constants */
typedef struct {
    int type;
    int initialized;
    union {
        int value;                  /* value of constant */
        float valueF;
        char valueC;
    };
    
} conNodeType;

/* identifiers */
typedef struct {
    char* i;                      /* subscript to sym array */
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

extern map<char*, conNodeType*> sym;
