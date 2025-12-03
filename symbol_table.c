#include "mini_c.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *name;
    int offset;
} Symbol;

Symbol table[100];
int tableIdx = 0;

void sym_insert(char *name, int offset) {
    for(int i=0; i<tableIdx; i++) {
        if(strcmp(table[i].name, name) == 0) return; // Already exists
    }
    table[tableIdx].name = strdup(name);
    table[tableIdx].offset = offset;
    tableIdx++;
}

int sym_lookup(char *name) {
    for(int i=0; i<tableIdx; i++) {
        if(strcmp(table[i].name, name) == 0) return table[i].offset;
    }
    return -1;
}

void sym_clear() {
    tableIdx = 0;
}