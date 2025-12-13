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
