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

// Instruction Structure
typedef struct {
    int opcode;
    int operand; // Generic int operand (value or address)
    char *strOperand; // For format strings or label names
} Instr;

Instr code[MAX_CODE];
int cpc = 0; // Code pointer

// VM Memory
// Changed to long long to hold both int values and 64-bit pointers (strings)
long long stack[MAX_STACK]; 
int sp = 0; // Stack pointer

int callStack[MAX_STACK];
int csp = 0; // Call stack pointer (for return addresses)

// Symbol Table for Runtime Variables
typedef struct { char *name; int val; } Var;
Var memory[MAX_VARS];
int varCount = 0;

// Label Table
typedef struct { char *name; int addr; } Label;
Label labels[200];
int lblCount = 0;

// Helper: Get or Create Variable Address
int get_var_idx(char *name) {
    for(int i=0; i<varCount; i++) {
        if(strcmp(memory[i].name, name) == 0) return i;
    }
    memory[varCount].name = strdup(name);
    memory[varCount].val = 0;
    return varCount++;
}

// Helper: Manage Labels
void set_label_addr(char *name, int addr) {
    for(int i=0; i<lblCount; i++) {
        if(strcmp(labels[i].name, name) == 0) {
            labels[i].addr = addr;
            return;
        }
    }
    labels[lblCount].name = strdup(name);
    labels[lblCount].addr = addr;
    lblCount++;
}

int get_label_addr(char *name) {
    for(int i=0; i<lblCount; i++) {
        if(strcmp(labels[i].name, name) == 0) return labels[i].addr;
    }
    return -1; // Should be patched later
}

// Emit Instructions
void emit(int op, int operand, char *str) {
    code[cpc].opcode = op;
    code[cpc].operand = operand;
    code[cpc].strOperand = str ? strdup(str) : NULL;
    cpc++;
}

// --- Generator ---
void generate_target_code() {
    printf("\n--- Generating & Executing VM Code ---\n");

    // Pass 1: Generate Instructions
    for(int i=0; i<qIdx; i++) {
        Quad q = quadList[i];
        
        // Label Definition
        if(q.op == Q_MOV && q.arg1 == NULL) {
            set_label_addr(q.res, cpc);
            continue;
        }
        if(q.op == Q_FUNC_START) {
            set_label_addr(q.arg1, cpc);
            continue;
        }

        // Params (Push to stack)
        if(q.op == Q_PARAM) {
            if(isdigit(q.arg1[0])) emit(I_PUSH, atoi(q.arg1), NULL);
            else if(q.arg1[0] == '"') { 
                // FIX: Strip quotes before emitting string literal
                char *clean = strdup(q.arg1 + 1); 
                if(strlen(clean) > 0) clean[strlen(clean)-1] = '\0'; 
                emit(I_PUSH, 0, clean); 
                free(clean);
            } 
            else { emit(I_LOAD, get_var_idx(q.arg1), NULL); }
            continue;
        }

        // Call
        if(q.op == Q_CALL) {
            if(strcmp(q.arg1, "printf") == 0) {
                // INTELLIGENT ARG COUNTING
                // Count how many params were pushed immediately before this call
                int argc = 0;
                for(int k = i-1; k >= 0; k--) {
                    if(quadList[k].op == Q_PARAM) argc++;
                    else break; // Stop if we hit a non-param
                }
                emit(I_PRINTF, argc, NULL); // Pass arg count to VM
            } else {
                emit(I_CALL, 0, q.arg1); 
            }
            if(q.res) emit(I_STORE, get_var_idx(q.res), NULL);
            continue;
        }

        // Return
        if(q.op == Q_RET) {
            if(isdigit(q.arg1[0])) emit(I_PUSH, atoi(q.arg1), NULL);
            else emit(I_LOAD, get_var_idx(q.arg1), NULL);
            emit(I_RET, 0, NULL);
            continue;
        }

        // Jumps
        if(q.op == Q_JZ) {
            emit(I_LOAD, get_var_idx(q.arg1), NULL);
            emit(I_JZ, 0, q.res); 
            continue;
        }
        if(q.op == Q_JMP) {
            emit(I_JMP, 0, q.res);
            continue;
        }

        // Arithmetic & Assignment
        if(q.op == Q_MOV) { 
            if(isdigit(q.arg1[0])) emit(I_PUSH, atoi(q.arg1), NULL);
            else emit(I_LOAD, get_var_idx(q.arg1), NULL);
            emit(I_STORE, get_var_idx(q.res), NULL);
            continue;
        }

        // Operations
        if(q.op >= Q_ADD && q.op <= Q_GT) {
            if(isdigit(q.arg1[0])) emit(I_PUSH, atoi(q.arg1), NULL);
            else emit(I_LOAD, get_var_idx(q.arg1), NULL);
            
            if(isdigit(q.arg2[0])) emit(I_PUSH, atoi(q.arg2), NULL);
            else emit(I_LOAD, get_var_idx(q.arg2), NULL);
            
            if(q.op == Q_ADD) emit(I_ADD, 0, NULL);
            if(q.op == Q_SUB) emit(I_SUB, 0, NULL);
            if(q.op == Q_MUL) emit(I_MUL, 0, NULL);
            if(q.op == Q_DIV) emit(I_DIV, 0, NULL);
            if(q.op == Q_EQ)  emit(I_EQ, 0, NULL);
            if(q.op == Q_NEQ) emit(I_NEQ, 0, NULL);
            if(q.op == Q_LT)  emit(I_LT, 0, NULL);
            if(q.op == Q_GT)  emit(I_GT, 0, NULL);
            
            emit(I_STORE, get_var_idx(q.res), NULL);
        }
    }
    emit(I_HALT, 0, NULL);

    // Pass 2: Patch Labels
    for(int i=0; i<cpc; i++) {
        if((code[i].opcode == I_CALL || code[i].opcode == I_JMP || code[i].opcode == I_JZ) && code[i].strOperand) {
            int addr = get_label_addr(code[i].strOperand);
            if(addr != -1) code[i].operand = addr;
            else printf("Warning: Label %s not found\n", code[i].strOperand);
        }
    }
}

// --- Run VM ---
void run_vm() {
    printf("--- Output ---\n");
    int pc = get_label_addr("main"); // Start at main
    if(pc == -1) pc = 0;
    
    int running = 1;
    while(running && pc < cpc) {
        Instr i = code[pc++];
        switch(i.opcode) {
            case I_PUSH: 
                if(i.strOperand) stack[sp++] = (long long)i.strOperand; 
                else stack[sp++] = i.operand; 
                break;
            case I_LOAD: stack[sp++] = memory[i.operand].val; break;
            case I_STORE: memory[i.operand].val = (int)stack[--sp]; break;
            case I_ADD: { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a+b; } break;
            case I_SUB: { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a-b; } break;
            case I_MUL: { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a*b; } break;
            case I_DIV: { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a/b; } break;
            case I_EQ:  { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a==b; } break;
            case I_NEQ: { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a!=b; } break;
            case I_LT:  { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a<b; } break;
            case I_GT:  { int b=stack[--sp]; int a=stack[--sp]; stack[sp++] = a>b; } break;
            
            case I_JZ: { 
                int cond = stack[--sp]; 
                if(cond == 0) pc = i.operand; 
            } break;
            case I_JMP: pc = i.operand; break;
            
            case I_CALL: {
                callStack[csp++] = pc;
                pc = i.operand;
            } break;
            
            case I_RET: {
                if(csp > 0) pc = callStack[--csp];
                else running = 0; // Return from main
            } break;

            case I_PRINTF: {
                int argc = i.operand;
                
                if (argc == 1) {
                    // Case: printf("Hello");
                    char *fmt = (char*)stack[--sp];
                    printf(fmt);
                    printf("\n");
                } 
                else if (argc == 2) {
                    // Case: printf("Val: %d", x);
                    // Arguments are pushed: [FmtStr] then [Val]
                    // So Stack Top is Val.
                    int val = (int)stack[--sp];
                    char *fmt = (char*)stack[--sp];
                    printf(fmt, val);
                    printf("\n");
                }
                else {
                    printf("VM Error: printf supports only 1 or 2 args in this demo.\n");
                }
            } break;
            
            case I_HALT: running = 0; break;
        }
    }
}