#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILE_NAME "data.txt"
#define MAX_LINE_LENGTH 100

void sort(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] > arr[j]) {
                int swap = arr[i];
                arr[i] = arr[j];
                arr[j] = swap;
            }
        }
    }
}

int main() {
    FILE *file;
    int array[MAX_LINE_LENGTH];
    int size;

    printf("Enter Number of elements (0 - 100): ");
    if(scanf("%d", &size) != 1) {
        printf("Invalid Input Format\n");
        return 1;
    }
    if(size <=0 || size > MAX_LINE_LENGTH) {
        printf("Number of Elements should be between 0 and 100");
        return 1;
    }

    printf("Enter elements:\n");
    for (int iterate = 0; iterate < size; iterate++)
    {
        if(scanf("%d", &array[iterate]) != 1) {
            printf("Invalid Input Format");
            return 1;
        }
    }

    printf("\nArray Before Sorting\n");
    for (int iterate = 0; iterate < size; iterate++) {
        printf("%d ", array[iterate]);
    }
    printf("\n");

    file = fopen(FILE_NAME, "w");
    if(file == NULL) {
        printf("Can not open file\n");
        return 1;
    }

    fprintf(file, "%d\n", size);
    for (int iterate = 0; iterate < size; iterate++) {
        fprintf(file, "%d ", array[iterate]);
    }

    fclose(file);

    pid_t processId = fork();

    if (processId == 0) {
        int tempArr[MAX_LINE_LENGTH];
        int tempSize;

        file = fopen(FILE_NAME, "r");
        if(file == NULL) {
            printf("File does not exist\n");
            return 1;
        }

        fscanf(file, "%d", &tempSize);
        for (int iterate = 0; iterate < tempSize; iterate++) {
            fscanf(file, "%d", &tempArr[iterate]);
        }
        fclose(file);

        sort(tempArr, tempSize);

        file = fopen(FILE_NAME, "w");
        if(file == NULL) {
            printf("Can not open file\n");
            return 1;
        }

        fprintf(file, "%d\n", size);
        for (int iterate = 0; iterate < size; iterate++) {
            fprintf(file, "%d ", tempArr[iterate]);
        }

        fclose(file);

        return 0;
    }

    wait(NULL);

    file = fopen(FILE_NAME, "r");
    if(file == NULL) {
        printf("Can not open file\n");
        return 1;
    }
    
    fscanf(file, "%d", &size);
    for (int iterate = 0; iterate < size; iterate++) {
        fscanf(file, "%d", &array[iterate]);
    }
    fclose(file);

    printf("Array After Sorting\n");
    for (int iterate = 0; iterate < size; iterate++) {
        printf("%d ", array[iterate]);
    }

    return 0;
}