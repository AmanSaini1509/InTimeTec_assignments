#include <stdio.h>
#include <stdlib.h>

struct studentDetail
{
    int rollNo;
    char name[10];
    int marks[3];
};

int calculateTotalMarks(int marks[], int sizeOfArray)
{
    int totalMarks = 0;
    for (int i = 0; i < sizeOfArray; i++) {
        totalMarks += marks[i];
    }
    return totalMarks;
}

char assignGrade(float averageMark)
{
    if (averageMark >= 85) return 'A';
    else if (averageMark >= 70) return 'B';
    else if (averageMark >= 50) return 'C';
    else if (averageMark >= 35) return 'D';
    else return 'F';
}

void displayPerformance(char grade)
{
    int numberOfStar = 0;
    switch (grade) {
        case 'A':
            numberOfStar = 5;
            break;
        case 'B':
            numberOfStar = 4;
            break;
        case 'C':
            numberOfStar = 3;
            break;
        case 'D':
            numberOfStar = 2;
            break;
        default:
            numberOfStar = 0;
            break;
    }
    for (int i = 0; i < numberOfStar; i++) {
        printf("%c", '*');
    }
}

void iterateStudents(struct studentDetail studentArray[], int sizeOfArray) {
    for (int i = 0; i < sizeOfArray; i++) {
        int sizeOfMarksArray = sizeof(studentArray[i].marks) / sizeof(studentArray[i].marks[0]);
        printf("Roll: %d\n", studentArray[i].rollNo);

        printf("Name: %s\n", studentArray[i].name);

        int totalMarks = calculateTotalMarks(studentArray[i].marks, sizeOfMarksArray);
        printf("Total: %d\n", totalMarks);

        const float averageMarks = (float)totalMarks / (float)sizeOfMarksArray;
        printf("Average: %.2f\n", averageMarks);

        char grade = assignGrade(averageMarks);
        printf("Grade: %c\n", grade);
        if (averageMarks < 35) continue;
        printf("Performance: ");
        displayPerformance(grade);
        printf("\n");
        printf("\n");
    }
}

void printRollNumber(struct studentDetail studentArray[], int lastIndexOfArray) {
    if (lastIndexOfArray == 0) {
        printf("%d\t",studentArray[lastIndexOfArray].rollNo);
        return;
    }
    printRollNumber(studentArray, lastIndexOfArray - 1);
    printf("%d\t",studentArray[lastIndexOfArray].rollNo);
}

int main()
{
    int numberOfStudents = 0;
    printf("Enter number of students:\t");
    scanf("%d", &numberOfStudents);
    if (numberOfStudents <= 0 || numberOfStudents > 100) {
        printf("Invalid Input, number of student must be between 0 and 100\n");
        exit(0);
    }
    struct studentDetail studentArray[numberOfStudents];
    for (int i = 0; i < numberOfStudents; i++) {
        printf("Enter student%d Roll no. | Name | marks of three subjects: ", i + 1);
        scanf("%d %s %d %d %d", &studentArray[i].rollNo, studentArray[i].name, &studentArray[i].marks[0], &studentArray[i].marks[1], &studentArray[i].marks[2]);
    }

    iterateStudents(studentArray, numberOfStudents);
    printf("\n");
    printf("List of Roll Numbers (via recursion):\t");
    printRollNumber(studentArray, numberOfStudents - 1);
   return 0;
}