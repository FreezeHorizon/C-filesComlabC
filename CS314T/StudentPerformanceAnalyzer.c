#include <stdio.h>
#include <stdlib.h>
#define MAX_AMOUNT_STUDENTS 100

struct Subject{
    char name[20];
    int score;
};

struct Student{
    char student_name[30];
    struct Subject SubjectStat;
};

int main(){
    printf("Enter Number of students (Max %d): ", MAX_AMOUNT_STUDENTS);
    int input;
    scanf("%d", &input);
    struct Student* Students;
    Students = (struct Student*)malloc(input * sizeof(struct Student));
    int i = (sizeof(Students)/sizeof(Students[0]));
    for(i; i > 0; i--){
        printf("num: %d",i);
    }
    free(Students);
}