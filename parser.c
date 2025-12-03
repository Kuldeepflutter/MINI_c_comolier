#include "mini_c.h"

Node *new_node(NodeType type) {
    Node *node = calloc(1, sizeof(Node));
    node->type = type;
    return node;
}

Node *expr();
Node *stmt();

Node *primary() {
    if (curTok.type == TOK_NUM) {
        Node *n = new_node(ND_NUM);
        n->val = curTok.numVal;
        next_token();
        return n;
    }
    // Handle String Literals
    if (curTok.type == TOK_STRING) {
        Node *n = new_node(ND_STR);
        n->strVal = curTok.str;
        next_token();
        return n;
    }
    if (curTok.type == TOK_ID) {
        char *name = curTok.str;
        next_token();
        // Function Call
        if (curTok.type == TOK_LPAREN) { 
            consume(TOK_LPAREN);
            Node *n = new_node(ND_CALL);
            n->name = name;
            Node *head = NULL, *curr = NULL;
            if (curTok.type != TOK_RPAREN) {
                head = expr();
                curr = head;
                while (curTok.type == TOK_COMMA) {
                    consume(TOK_COMMA);
                    curr->next = expr();
                    curr = curr->next;
                }
            }
            n->args = head;
            consume(TOK_RPAREN);
            return n;
        }
        // Variable
        Node *n = new_node(ND_VAR);
        n->name = name;
        return n;
    }
    if (curTok.type == TOK_LPAREN) {
        consume(TOK_LPAREN);
        Node *n = expr();
        consume(TOK_RPAREN);
        return n;
    }
    return NULL;
}

Node *mul() {
    Node *lhs = primary();
    while (curTok.type == TOK_MUL || curTok.type == TOK_DIV) {
        NodeType type = (curTok.type == TOK_MUL) ? ND_MUL : ND_DIV;
        next_token();
        Node *n = new_node(type);
        n->lhs = lhs;
        n->rhs = primary();
        lhs = n;
    }
    return lhs;
}

Node *add() {
    Node *lhs = mul();
    while (curTok.type == TOK_ADD || curTok.type == TOK_SUB) {
        NodeType type = (curTok.type == TOK_ADD) ? ND_ADD : ND_SUB;
        next_token();
        Node *n = new_node(type);
        n->lhs = lhs;
        n->rhs = mul();
        lhs = n;
    }
    return lhs;
}

Node *relational() {
    Node *lhs = add();
    while (curTok.type == TOK_LT || curTok.type == TOK_GT || curTok.type == TOK_EQ || curTok.type == TOK_NEQ) {
        NodeType type;
        if (curTok.type == TOK_LT) type = ND_LT;
        else if (curTok.type == TOK_GT) type = ND_GT;
        else if (curTok.type == TOK_EQ) type = ND_EQ;
        else type = ND_NEQ;
        next_token();
        Node *n = new_node(type);
        n->lhs = lhs;
        n->rhs = add();
        lhs = n;
    }
    return lhs;
}

Node *expr() { return relational(); }

Node *stmt() {
    if (curTok.type == TOK_INT) {
        consume(TOK_INT);
        char *name = curTok.str;
        consume(TOK_ID);
        consume(TOK_ASSIGN);
        Node *n = new_node(ND_ASSIGN);
        n->name = name;
        n->rhs = expr();
        consume(TOK_SEMI);
        return n;
    }
    if (curTok.type == TOK_IF) {
        consume(TOK_IF);
        consume(TOK_LPAREN);
        Node *n = new_node(ND_IF);
        n->cond = expr();
        consume(TOK_RPAREN);
        n->then = stmt();
        if (curTok.type == TOK_ELSE) {
            consume(TOK_ELSE);
            n->els = stmt();
        }
        return n;
    }
    if (curTok.type == TOK_WHILE) {
        consume(TOK_WHILE);
        consume(TOK_LPAREN);
        Node *n = new_node(ND_WHILE);
        n->cond = expr();
        consume(TOK_RPAREN);
        n->body = stmt();
        return n;
    }
    if (curTok.type == TOK_RETURN) {
        consume(TOK_RETURN);
        Node *n = new_node(ND_RETURN);
        n->lhs = expr();
        consume(TOK_SEMI);
        return n;
    }
    if (curTok.type == TOK_LBRACE) {
        consume(TOK_LBRACE);
        Node *head = NULL, *curr = NULL;
        while (curTok.type != TOK_RBRACE) {
            Node *s = stmt();
            if (s) {
                if(!head) head = s;
                else curr->next = s;
                curr = s;
            }
        }
        consume(TOK_RBRACE);
        Node *n = new_node(ND_BLOCK);
        n->body = head;
        return n;
    }
    
    // Handle standalone expressions (assignments, function calls)
    if (curTok.type == TOK_ID) {
        char *name = curTok.str;
        consume(TOK_ID);
        
        if (curTok.type == TOK_LPAREN) { 
            // Function Call stmt: printf(...);
            consume(TOK_LPAREN);
            Node *n = new_node(ND_CALL);
            n->name = name;
            Node *head = NULL, *curr = NULL;
            if (curTok.type != TOK_RPAREN) {
                head = expr();
                curr = head;
                while (curTok.type == TOK_COMMA) {
                    consume(TOK_COMMA);
                    curr->next = expr();
                    curr = curr->next;
                }
            }
            n->args = head;
            consume(TOK_RPAREN);
            consume(TOK_SEMI); 
            return n;
        } else {
            // Assignment: a = 10;
            consume(TOK_ASSIGN);
            Node *n = new_node(ND_ASSIGN);
            n->name = name;
            n->rhs = expr();
            consume(TOK_SEMI);
            return n;
        }
    }

    return NULL;
}

Node *parse_program() {
    Node *head = NULL, *curr = NULL;
    while(curTok.type != TOK_EOF) {
        // Simple heuristic: assume everything at top level is a function "int name()..."
        // This mini-compiler assumes 'int' starts a function definition.
        consume(TOK_INT);
        char *name = curTok.str;
        consume(TOK_ID);
        consume(TOK_LPAREN);
        
        Node *args = NULL, *acurr = NULL;
        if(curTok.type != TOK_RPAREN) {
            while(1) {
                consume(TOK_INT);
                char *aname = curTok.str;
                consume(TOK_ID);
                Node *arg = new_node(ND_VAR);
                arg->name = aname;
                if(!args) args = arg; else acurr->next = arg;
                acurr = arg;
                if(curTok.type == TOK_COMMA) consume(TOK_COMMA);
                else break;
            }
        }
        consume(TOK_RPAREN);
        Node *body = stmt();
        
        Node *func = new_node(ND_FUNC);
        func->name = name;
        func->args = args;
        func->body = body;
        
        if(!head) head = func; else curr->next = func;
        curr = func;
    }
    return head;
}