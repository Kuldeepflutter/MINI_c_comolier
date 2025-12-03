
int rand(){
    return 122;
}

int main() {
    int i = 0;
    while(i < 5) {
        i = i + 1;
        printf("Counting: %d", i);
    }
    
    int a = 10;
    if (a > 5) {
        a=rand();
        printf("A is big: %d", a);
    } else {
        a=rand();
        printf("A is small: %d", a);
    }
    
    return 0;
}