#include <stdio.h>
#include <stdlib.h>

const int GRADE_A_THRESHOLD = 85;
const int GRADE_B_THRESHOLD = 70;
const int GRADE_C_THRESHOLD = 50;
const int GRADE_D_THRESHOLD = 35;

struct studentDetail //structure for Student Detail
{
    int rollNo;
    char name[10];
    int marks[3];
};

int calculateTotalMarks(int marks[], int sizeOfArray) // function to calculate the total marks
{
    int totalMarks = 0;
    for (int iterator = 0; iterator < sizeOfArray; iterator++) {
        totalMarks += marks[iterator];
    }
    return totalMarks;
}

char assignGrade(const float averageMark) //function to assign grade based on averageMarks
{
    if (averageMark >= GRADE_A_THRESHOLD) return 'A';
    else if (averageMark >= GRADE_B_THRESHOLD) return 'B';
    else if (averageMark >= GRADE_C_THRESHOLD) return 'C';
    else if (averageMark >= GRADE_D_THRESHOLD) return 'D';
    else return 'F';
}

void displayPerformance(const char grade) // function to display performance
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
    for (int iterator = 0; iterator < numberOfStar; iterator++) {
        printf("%c", '*');
    }
}

// function to display performances of students
void displayStudentPerformances(struct studentDetail studentArray[], const int sizeOfArray)
{
    for (int iterator = 0; iterator < sizeOfArray; iterator++) {
        //calculating size of array of marks
        const int sizeOfMarksArray = sizeof(studentArray[iterator].marks) / sizeof(studentArray[iterator].marks[0]);

        printf("Roll: %d\n", studentArray[iterator].rollNo); // roll no.

        printf("Name: %s\n", studentArray[iterator].name); // name

        const int totalMarks = calculateTotalMarks(studentArray[iterator].marks, sizeOfMarksArray);
        printf("Total: %d\n", totalMarks); // total marks

        const float averageMarks = (float)totalMarks / (float)sizeOfMarksArray;
        printf("Average: %.2f\n", averageMarks); // average marks in float value

        char grade = assignGrade(averageMarks);
        printf("Grade: %c\n", grade); // grade

        if (averageMarks < 35) continue;
        printf("Performance: ");
        displayPerformance(grade); // performance (print *)
        printf("\n");
        printf("\n");
    }
}

//print roll numbers using recursion
void printRollNumber(struct studentDetail studentArray[], int lastIndexOfArray)
{
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

    struct studentDetail studentArray[numberOfStudents]; // array for storing multiple student records
    for (int iterator = 0; iterator < numberOfStudents; iterator++) {
        printf("Enter student%d Roll no. | Name | marks of three subjects: ", iterator + 1);
        scanf("%d %s %d %d %d", &studentArray[iterator].rollNo, studentArray[iterator].name, &studentArray[iterator].marks[0], &studentArray[iterator].marks[1], &studentArray[iterator].marks[2]);
    }

    displayStudentPerformances(studentArray, numberOfStudents);
    printf("\n");
    printf("List of Roll Numbers (via recursion):\t");
    printRollNumber(studentArray, numberOfStudents - 1);
   return 0;
}