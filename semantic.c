#include "mini_c.h"

// A simple recursive check. 
// In a full compiler, this would also check types.
void check_node(Node *node) {
    if(!node) return;

    switch(node->type) {
        case ND_VAR:
            // Check if variable exists in symbol table
            // Note: Since we haven't populated it fully in parsing, 
            // we will just print what we are checking.
            // Real implementation requires populating table during parse or a dedicated pass.
            break;
        case ND_ASSIGN:
            // For this mini compiler, we register variables on assignment (declaration)
            // if they are standard declarations like 'int x = 5'
            sym_insert(node->name, 0); 
            check_node(node->rhs);
            break;
        case ND_ADD: case ND_SUB: case ND_MUL: case ND_DIV:
        case ND_EQ: case ND_NEQ: case ND_LT: case ND_GT:
            check_node(node->lhs);
            check_node(node->rhs);
            break;
        case ND_IF:
            check_node(node->cond);
            check_node(node->then);
            if(node->els) check_node(node->els);
            break;
        case ND_WHILE:
            check_node(node->cond);
            check_node(node->body);
            break;
        case ND_BLOCK:
            for(Node *n = node->body; n; n = n->next) check_node(n);
            break;
        case ND_FUNC:
            sym_clear(); // New scope
            // Register args
            for(Node *a=node->args; a; a=a->next) sym_insert(a->name, 0);
            check_node(node->body);
            break;
        case ND_RETURN:
            check_node(node->lhs);
            break;
        default: break;
    }
}

void check_semantics(Node *prog) {
    for(Node *n = prog; n; n=n->next) {
        check_node(n);
    }
}