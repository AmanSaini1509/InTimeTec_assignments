#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ELEMENT 100

void sortArray(int *arr, int size) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = i + 1; j < n; j++) {
            if(arr[i] > arr[j]) {
                int swap = arr[i];
                arr[i] = arr[j];
                arr[j] = swap;
            }
        }
    }
}

int main()
{
    int pipe1[2];
    int pipe2[2];
    int arr[MAX_ELEMENT];
    int size;

    pipe(pipe1);
    pipe(pipe2);

    pid_t processId = fork();

    if(processId > 0)
    {
        close(pipe1[0]);
        close(pipe2[1]);

        printf("Enter number of elements: ");
        if (scanf("%d", &size) != 1) {
            printf("Invalid Input Format\n");
            return 1;
        }
        if(size <= 0 || size > MAX_ELEMENT) {
            printf("Invalid input Range\n");
            return 1;
        }

        printf("Enter elements:\n");
        for(int i = 0; i < size; i++) {
            if (scanf("%d", &arr[i]) != 1) {
               printf("Invalid Input Format\n");
               return 1;
           }
        }

        printf("\nArray before sorting:\n");
        for(int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        write(pipe1[1], &size, sizeof(size));
        write(pipe1[1], arr, size * sizeof(int));

        read(pipe2[0], arr, size * sizeof(int));

        printf("\nArray after sorting:\n");
        for(int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);
    }
    else
    {
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], &size, sizeof(size));
        read(pipe1[0], arr, size * sizeof(int));

        sortArray(arr, size);

        write(pipe2[1], arr, size * sizeof(int));

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}
