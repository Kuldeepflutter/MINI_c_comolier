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
     
