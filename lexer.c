#include "mini_c.h"

char *src;
Token curTok;

void next_token() {
    while (*src) {
        if (isspace(*src)) { src++; continue; }
        
        // Skip Comments //
        if (*src == '/' && *(src+1) == '/') {
            while (*src && *src != '\n') src++;
            continue;
        }

        // Skip Preprocessor #
        if (*src == '#') {
            while (*src && *src != '\n') src++;
            continue;
        }

        break;
    }

    if (*src == 0) { curTok.type = TOK_EOF; return; }

    // Handle Numbers
    if (isdigit(*src)) {
        curTok.type = TOK_NUM;
        curTok.numVal = 0;
        while (isdigit(*src)) curTok.numVal = curTok.numVal * 10 + (*src++ - '0');
        return;
    }

    // Handle Strings (for printf)
    if (*src == '"') {
        src++; // skip opening quote
        char *start = src;
        while (*src && *src != '"') src++;
        int len = src - start;
        char *str = malloc(len + 1);
        strncpy(str, start, len);
        str[len] = '\0';
        src++; // skip closing quote
        
        curTok.type = TOK_STRING;
        curTok.str = str;
        return;
    }

    // Handle Identifiers and Keywords
    if (isalpha(*src)) {
        char *start = src;
        while (isalnum(*src) || *src == '_') src++;
        int len = src - start;
        char *word = malloc(len + 1);
        strncpy(word, start, len);
        word[len] = '\0';

        if (strcmp(word, "int") == 0) curTok.type = TOK_INT;
        else if (strcmp(word, "if") == 0) curTok.type = TOK_IF;
        else if (strcmp(word, "else") == 0) curTok.type = TOK_ELSE;
        else if (strcmp(word, "while") == 0) curTok.type = TOK_WHILE;
        else if (strcmp(word, "return") == 0) curTok.type = TOK_RETURN;
        else {
            curTok.type = TOK_ID;
            curTok.str = word;
        }
        return;
    }

    // Handle Symbols
    switch (*src++) {
        case '(': curTok.type = TOK_LPAREN; break;
        case ')': curTok.type = TOK_RPAREN; break;
        case '{': curTok.type = TOK_LBRACE; break;
        case '}': curTok.type = TOK_RBRACE; break;
        case ';': curTok.type = TOK_SEMI; break;
        case ',': curTok.type = TOK_COMMA; break;
        case '+': curTok.type = TOK_ADD; break;
        case '-': curTok.type = TOK_SUB; break;
        case '*': curTok.type = TOK_MUL; break;
        case '/': curTok.type = TOK_DIV; break;
        case '=':
            if (*src == '=') { src++; curTok.type = TOK_EQ; }
            else curTok.type = TOK_ASSIGN;
            break;
        case '!':
            if (*src == '=') { src++; curTok.type = TOK_NEQ; }
            else { printf("Lexer Error: Unexpected char '!'\n"); exit(1); }
            break;
        case '<': curTok.type = TOK_LT; break;
        case '>': curTok.type = TOK_GT; break;
        default: printf("Lexer Error: Unknown char '%c'\n", *(src-1)); exit(1);
    }
}

void consume(TokenType type) {
    if (curTok.type != type) {
        printf("Parser Error: Expected token type %d, got %d", type, curTok.type);
        // Debugging Aid: Print the value of the unexpected token
        if (curTok.type == TOK_ID || curTok.type == TOK_STRING) {
            printf(" (Value: '%s')\n", curTok.str);
        } else if (curTok.type == TOK_NUM) {
            printf(" (Value: %d)\n", curTok.numVal);
        } else {
            printf("\n");
        }
        exit(1);
    }
    next_token();
}