#include "mini_c.h"

Quad quadList[1000];
int qIdx = 0;
int tempIdx = 0;
int lblIdx = 0;

char* new_temp() {
    char *buf = malloc(10);
    sprintf(buf, "t%d", tempIdx++);
    return buf;
}

char* new_label() {
    char *buf = malloc(10);
    sprintf(buf, "L%d", lblIdx++);
    return buf;
}

void emit_quad(QuadOp op, char *a1, char *a2, char *res) {
    quadList[qIdx].op = op;
    quadList[qIdx].arg1 = a1 ? strdup(a1) : NULL;
    quadList[qIdx].arg2 = a2 ? strdup(a2) : NULL;
    quadList[qIdx].res = res ? strdup(res) : NULL;
    qIdx++;
}

char* gen_temp_node(Node *node) {
    if(!node) return NULL;
    
    if(node->type == ND_NUM) {
        char *buf = malloc(10);
        sprintf(buf, "%d", node->val);
        return buf;
    }
    
    // FIX: Wrap string literals in quotes so Codegen can distinguish them from vars
    if(node->type == ND_STR) {
        char *buf = malloc(strlen(node->strVal) + 3);
        sprintf(buf, "\"%s\"", node->strVal);
        return buf;
    }

    if(node->type == ND_VAR) return node->name;

    if(node->type >= ND_ADD && node->type <= ND_GT) {
        char *t1 = gen_temp_node(node->lhs);
        char *t2 = gen_temp_node(node->rhs);
        char *res = new_temp();
        QuadOp op;
        switch(node->type) {
            case ND_ADD: op = Q_ADD; break;
            case ND_SUB: op = Q_SUB; break;
            case ND_MUL: op = Q_MUL; break;
            case ND_DIV: op = Q_DIV; break;
            case ND_LT:  op = Q_LT; break;
            case ND_GT:  op = Q_GT; break;
            case ND_EQ:  op = Q_EQ; break;
            case ND_NEQ: op = Q_NEQ; break;
            default: op = Q_ADD; break;
        }
        emit_quad(op, t1, t2, res);
        return res;
    }
    
    if(node->type == ND_CALL) {
        // Push args
        for(Node *a = node->args; a; a = a->next) {
            char *t = gen_temp_node(a);
            emit_quad(Q_PARAM, t, NULL, NULL);
        }
        char *res = new_temp();
        emit_quad(Q_CALL, node->name, NULL, res);
        return res;
    }
    return NULL;
}

void generate_icg(Node *node) {
    if(!node) return;

    if(node->type == ND_FUNC) {
        emit_quad(Q_FUNC_START, node->name, NULL, NULL);
        generate_icg(node->body);
        emit_quad(Q_FUNC_END, NULL, NULL, NULL);
    }
    else if(node->type == ND_BLOCK) {
        for(Node *n = node->body; n; n = n->next) generate_icg(n);
    }
    else if(node->type == ND_ASSIGN) {
        char *t = gen_temp_node(node->rhs);
        emit_quad(Q_MOV, t, NULL, node->name);
    }
    else if(node->type == ND_CALL) {
        // Standalone function call (e.g., printf)
        for(Node *a = node->args; a; a = a->next) {
            char *t = gen_temp_node(a);
            emit_quad(Q_PARAM, t, NULL, NULL);
        }
        emit_quad(Q_CALL, node->name, NULL, NULL); // No result needed
    }
    else if(node->type == ND_RETURN) {
        char *t = gen_temp_node(node->lhs);
        emit_quad(Q_RET, t, NULL, NULL);
    }
    else if(node->type == ND_IF) {
        char *cond = gen_temp_node(node->cond);
        char *elseLbl = new_label();
        char *endLbl = new_label();
        emit_quad(Q_JZ, cond, NULL, elseLbl); // Jump if zero to else
        generate_icg(node->then);
        emit_quad(Q_JMP, NULL, NULL, endLbl);
        emit_quad(Q_MOV, NULL, NULL, elseLbl); // Label position
        if(node->els) generate_icg(node->els);
        emit_quad(Q_MOV, NULL, NULL, endLbl); // Label position
    }
    else if(node->type == ND_WHILE) {
        char *startLbl = new_label();
        char *endLbl = new_label();
        emit_quad(Q_MOV, NULL, NULL, startLbl); // Label start
        char *cond = gen_temp_node(node->cond);
        emit_quad(Q_JZ, cond, NULL, endLbl);
        generate_icg(node->body);
        emit_quad(Q_JMP, NULL, NULL, startLbl);
        emit_quad(Q_MOV, NULL, NULL, endLbl); // Label end
    }
}

void print_icg() {
    printf("--- Intermediate Code (Quadruples) ---\n");
    for(int i=0; i<qIdx; i++) {
        Quad q = quadList[i];
        const char *opStr = "?";
        switch(q.op) {
            case Q_MOV: if(q.arg1) printf("%s = %s\n", q.res, q.arg1); else printf("%s:\n", q.res); continue; 
            case Q_ADD: opStr = "+"; break;
            case Q_SUB: opStr = "-"; break;
            case Q_MUL: opStr = "*"; break;
            case Q_DIV: opStr = "/"; break;
            case Q_EQ:  opStr = "=="; break;
            case Q_LT:  opStr = "<"; break;
            case Q_GT:  opStr = ">"; break;
            case Q_NEQ: opStr = "!="; break;
            case Q_JZ:  printf("IFZ %s GOTO %s\n", q.arg1, q.res); continue;
            case Q_JMP: printf("GOTO %s\n", q.res); continue;
            case Q_PARAM: printf("PARAM %s\n", q.arg1); continue;
            case Q_CALL: 
                if(q.res) printf("%s = CALL %s\n", q.res, q.arg1);
                else printf("CALL %s\n", q.arg1); 
                continue;
            case Q_RET: printf("RETURN %s\n", q.arg1); continue;
            case Q_FUNC_START: printf("FUNC %s:\n", q.arg1); continue;
            case Q_FUNC_END: printf("END FUNC\n"); continue;
            default: break;
        }
        printf("%s = %s %s %s\n", q.res, q.arg1, opStr, q.arg2);
    }
}