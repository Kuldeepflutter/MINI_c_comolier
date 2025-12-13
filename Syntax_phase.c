#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *input;
int lookahead;

void error() {
    printf("Syntax Error!\n");
    exit(1);
}

void match(int t) {
    if (lookahead == t) {
        lookahead = *input++;
    } else {
        error();
    }
}

// Grammar:
// stmt   -> id = expr ;
// expr   -> expr + term | expr - term | term
// term   -> term * factor | term / factor | factor
// factor -> ( expr ) | id | number

void F();
void T();
void E();

void F() {
    if (isalpha(lookahead)) {   // id
        match(lookahead);
    } else if (isdigit(lookahead)) { // number
        match(lookahead);
    } else if (lookahead == '(') {
        match('(');
        E();
        match(')');
    } else {
        error();
    }
}

void Tprime() {
    if (lookahead == '*') {
        match('*');
        F();
        Tprime();
    } else if (lookahead == '/') {
        match('/');
        F();
        Tprime();
    }
}

void T() {
    F();
    Tprime();
}

void Eprime() {
    if (lookahead == '+') {
        match('+');
        T();
        Eprime();
    } else if (lookahead == '-') {
        match('-');
        T();
        Eprime();
    }
}

void E() {
    T();
    Eprime();
}

void stmt() {
    if (isalpha(lookahead)) { // id = expr ;
        match(lookahead);
        if (lookahead == '=') {
            match('=');
            E();
            if (lookahead == ';') {
                match(';');
                printf("Syntax Correct!\n");
            } else {
                error();
            }
        } else {
            error();
        }
    } else {
        error();
    }
}

int main() {
    char expr[100];
    printf("Enter a statement (like a=3+5;): ");
    scanf("%s", expr);
    input = expr;
    lookahead = *input++;
    stmt();
    return 0;
}