#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 100
#define MAX_NAME_LEN 32

typedef enum {
    TYPE_INT,
    TYPE_UNDEFINED
} Type;

typedef struct {
    char name[MAX_NAME_LEN];
    Type type;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

int findSymbol(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void declareVariable(const char *name, Type type) {
    if (findSymbol(name) != -1) {
        // Variable already declared - ignoring silently
        return;
    }
    if (symbolCount >= MAX_SYMBOLS) {
        // Symbol table full - ignoring silently
        return;
    }
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].type = type;
    symbolCount++;
}

Type getVariableType(const char *name) {
    int pos = findSymbol(name);
    if (pos == -1) {
        // Variable not declared - return undefined silently
        return TYPE_UNDEFINED;
    }
    return symbolTable[pos].type;
}

void checkAssignment(const char *name, Type exprType) {
    Type varType = getVariableType(name);
    if (varType == TYPE_UNDEFINED) {
        // Variable not declared - ignore silently
        return;
    }
    if (varType != exprType) {
        // Type mismatch - ignore silently
        return;
    }
}

int main() {
    // Semantic analysis simulation:

    // Declare some variables
    declareVariable("x", TYPE_INT);
    declareVariable("y", TYPE_INT);

    // Attempt redeclaration (ignored silently)
    declareVariable("x", TYPE_INT);

    // Check assignments silently
    checkAssignment("x", TYPE_INT); 
    checkAssignment("y", TYPE_UNDEFINED); 
    checkAssignment("z", TYPE_INT); 

    // Simulating a correct assignment
    checkAssignment("x", TYPE_INT);

    return 0;
}