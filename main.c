#include "mini_c.h"

char *read_file(char *filename) {
    // Open in binary mode ("rb") to prevent Windows \r\n conversion issues
    // which cause ftell() size to mismatch fread() counts.
    FILE *fp = fopen(filename, "rb"); 
    if (!fp) { printf("Error: Cannot open %s\n", filename); exit(1); }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    
    char *buf = malloc(size + 1);
    if (!buf) { printf("Error: Memory allocation failed\n"); exit(1); }
    
    // Read exact bytes
    fread(buf, 1, size, fp);
    buf[size] = 0; // Null-terminate safely
    
    fclose(fp);
    return buf;
}

int main(int argc, char **argv) {
    if(argc < 2) { printf("Usage: ./mini_c test.c\n"); return 1; }
    
    src = read_file(argv[1]);
    next_token(); // 1. Lexing Start

    printf("[1/5] Lexing complete.\n");
    
    printf("[2/5] Parsing...\n");
    Node *prog = parse_program(); 
    
    printf("[3/5] Semantic Analysis...\n");
    check_semantics(prog); 

    printf("[4/5] Intermediate Code Generation...\n");
    for(Node *n = prog; n; n=n->next) generate_icg(n); 
 
    printf("[5/5] Running VM...\n");
    generate_target_code();
    run_vm();
    
    return 0;
}