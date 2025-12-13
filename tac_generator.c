// tac_generator.c
#include <stdio.h>
#include <string.h>

struct TAC {
    char op[4];
    char arg1[8];
    char arg2[8];
    char result[8];
};

int main() {
    char expr[100];
    struct TAC code[3];
    char left, x, y, z, op1, op2;
    int i;

    printf("Enter expression (example: a=b+c*d): ");
    scanf("%99s", expr);

    if (strlen(expr) < 7) {
        printf("Invalid expression format. Use something like a=b+c*d\n");
        return 0;
    }

    // Very naive parsing, assumes exactly: a=b+c*d
    left = expr[0];   // a
    x    = expr[2];   // b
    op2  = expr[3];   // + or -
    y    = expr[4];   // c
    op1  = expr[5];   // * or /
    z    = expr[6];   // d

    // t1 = y op1 z
    code[0].op[0] = op1; code[0].op[1] = '\0';
    code[0].arg1[0] = y;  code[0].arg1[1] = '\0';
    code[0].arg2[0] = z;  code[0].arg2[1] = '\0';
    strcpy(code[0].result, "t1");

    // t2 = x op2 t1
    code[1].op[0] = op2; code[1].op[1] = '\0';
    code[1].arg1[0] = x;  code[1].arg1[1] = '\0';
    strcpy(code[1].arg2, "t1");
    strcpy(code[1].result, "t2");

    // left = t2
    strcpy(code[2].op, "=");
    strcpy(code[2].arg1, "t2");
    code[2].arg2[0] = '\0';
    code[2].result[0] = left;
    code[2].result[1] = '\0';

    printf("\nThree-address code:\n");
    for (i = 0; i < 3; ++i) {
        if (strcmp(code[i].op, "=") == 0) {
            printf("%s = %s\n", code[i].result, code[i].arg1);
        } else {
            printf("%s = %s %s %s\n",
                   code[i].result,
                   code[i].arg1,
                   code[i].op,
                   code[i].arg2);
        }
    }

    return 0;
}
