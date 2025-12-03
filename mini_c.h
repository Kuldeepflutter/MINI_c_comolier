#ifndef MINI_C_H
#define MINI_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- LEXER ---
typedef enum {
    TOK_INT, TOK_ID, TOK_NUM, TOK_STRING, // Added TOK_STRING
    TOK_IF, TOK_ELSE, TOK_WHILE, TOK_RETURN,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_SEMI, TOK_COMMA, TOK_ASSIGN,
    TOK_EQ, TOK_NEQ, TOK_LT, TOK_GT, TOK_ADD, TOK_SUB, TOK_MUL, TOK_DIV,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *str;
    int numVal;
} Token;

extern Token curTok;
extern char *src;
void next_token();
void consume(TokenType type);

// --- AST NODES ---
typedef enum {
    ND_NUM, ND_STR, ND_VAR, ND_ASSIGN, ND_RETURN, // Added ND_STR
    ND_IF, ND_WHILE, ND_BLOCK, ND_FUNC, ND_CALL,
    ND_ADD, ND_SUB, ND_MUL, ND_DIV, ND_EQ, ND_NEQ, ND_LT, ND_GT
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *lhs, *rhs;
    struct Node *cond, *then, *els, *body, *next;
    struct Node *args; // For functions
    char *name;
    int val;
    char *strVal; // For string literals
} Node;

Node* parse_program();

// --- SYMBOL TABLE ---
void sym_insert(char *name, int offset);
int sym_lookup(char *name);
void sym_clear(); 

// --- SEMANTIC ANALYSIS ---
void check_semantics(Node *node);

// --- INTERMEDIATE CODE GENERATION (ICG) ---
typedef enum {
    Q_MOV, Q_ADD, Q_SUB, Q_MUL, Q_DIV, 
    Q_EQ, Q_NEQ, Q_LT, Q_GT,
    Q_JMP, Q_JZ, Q_PARAM, Q_CALL, Q_RET, Q_FUNC_START, Q_FUNC_END
} QuadOp;

typedef struct {
    QuadOp op;
    char *arg1; 
    char *arg2; 
    char *res;  
} Quad;

extern Quad quadList[1000];
extern int qIdx;
void generate_icg(Node *node);
void print_icg();

// --- CODEGEN / VM ---
void generate_target_code(); 
void run_vm(); // New function to execute the code

#endif