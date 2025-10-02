#include <stdio.h>

int multiply(int n, int result) {
    if (n == 0) {
        return 0; // Error: negative multiplier
    }
    if (n == 1) {
        return result;
    }
    int positive = n > 0;
    for(int i = 0; i < (positive ? n : -n); i++) {
        result += result;
    }
    return positive ? result : -result;
}

int main() {
    printf("Multiplicand: ");
    int n = scanf("%d", &n);
    printf("Multiplier: ");
    int m = scanf("%d", &m);
    int product = multiply(n, m);
    printf("Product: %d\n", product);
    return 0;

}