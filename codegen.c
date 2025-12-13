#include "mini_c.h"

// --- Virtual Machine ---
// A simple stack-based VM to execute the Quadruples directly.

#define MAX_CODE 2048
#define MAX_STACK 1024
#define MAX_VARS 200

// Instruction Set
enum { 
    I_PUSH, I_LOAD, I_STORE, 
    I_ADD, I_SUB, I_MUL, I_DIV, 
    I_EQ, I_NEQ, I_LT, I_GT,
    I_JZ, I_JMP, 
    I_CALL, I_RET, 
    I_HALT, I_PRINTF 
};

