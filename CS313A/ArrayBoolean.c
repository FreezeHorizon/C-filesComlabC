#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ARRAY_SIZE 20

int ArrayMaker(char* MainArray);
bool ArrayChecker(char* MainArray, int n);
void insertionSort(int arr[], int n);

void clearConsole() {
    system("cls");
}

int main() {
    char MainArray[MAX_ARRAY_SIZE];
    printf("Enter Numbers in array [Press X/x to exit]\n");
    printf("[_]");

    int count = ArrayMaker(MainArray);

    printf("Is Array Sorted? %s\n", ArrayChecker(MainArray, count) ? "Yes" : "No");

    return 0;
}

bool ArrayChecker(char* MainArray, int n) {
    if (n <= 1) {
        return true;
    }
    for (int i = 0; i < n - 1; i++) {
        if ((MainArray[i] - '0') > (MainArray[i+1] - '0')) {
            return false;
        }
    }
    return true;
}

void insertionSort(int arr[], int n)
{
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int ArrayMaker(char* MainArray) {
    int count = 0;

    while (count < MAX_ARRAY_SIZE) {
        char ch = getch();

        if (ch == 'x' || ch == 'X') {
            break;
        }
        if (isdigit(ch)) {
            MainArray[count++] = ch;
        } else {
            printf("\nWrong Input, Enter a number or 'x' to exit\n");
            continue;
        }

        clearConsole();
        printf("Enter Numbers in array [Press X/x to exit]\n");
        for (int i = 0; i < count; i++) {
            printf("[%c] ", MainArray[i]);
        }

        if (count < MAX_ARRAY_SIZE) {
            printf("[_]");
        }
        printf("\n");
    }

    printf("\nFinal Input: ");
    for (int i = 0; i < count; i++) {
        printf("[%c] ", MainArray[i]);
    }
    printf("\n");

    return count;
}