#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING 100

int main(){

    char* inputString;
    
    inputString = (char*) malloc((MAX_STRING + 1) * sizeof(char));

    printf("Input a string (max %d characters): ", MAX_STRING);
    scanf("%99[^\n]", inputString);
    inputString[strcspn(inputString, "\n")] = '\0';

    int length = strlen(inputString);
    
    printf("Original string: %s\n", inputString);
    printf("Reversed string: ");
    for(int i = length - 1; i >= 0; i--){
        printf("%c", *(inputString + 1)); 
    }
    free(inputString);
    inputString = NULL;
    return 0;
}