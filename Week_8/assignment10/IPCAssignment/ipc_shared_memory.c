#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ELEMENTS 100

struct shared_data {
    int length;
    int arr[MAX_ELEMENTS];
};

void sortArray(int *arr, int size) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = i + 1; j < size; j++) {
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
    key_t key;
    int shmId;
    struct shared_data *sharedMemory;

    FILE *fp = fopen("shmfile", "a");
    fclose(fp);

    key = ftok("shmfile", 65);
    shmId = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);

    sharedMemory = (struct shared_data*) shmat(shmId, NULL, 0);

    printf("Enter number of elements: ");
    if(scanf("%d", &sharedMemory->length) != 1) {
        printf("Invlaid Input Format\n");
        return 1;
    }
    if(sharedMemory->length <= 0 || sharedMemory->length > MAX_ELEMENTS) {
        printf("Invalid input range!\n");
        return 1;
    }

    printf("Enter elements:\n");
    for(int i = 0; i < sharedMemory->length; i++) {
        if(scanf("%d", &sharedMemory->arr[i]) != 1) {
            printf("Invalid Input Format\n");
            return 1;
        }
    }

    printf("\nArray before sorting:\n");
    for(int i = 0; i < sharedMemory->length; i++) {
        printf("%d ", sharedMemory->arr[i]);
    }

    pid_t processId = fork();

    if(processId == 0)
    {
        sortArray(sharedMemory->arr, sharedMemory->length);

        shmdt(sharedMemory);
        return 1;
    }
    wait(NULL);

    printf("\nArray after sorting:\n");
    for(int i = 0; i < sharedMemory->length; i++) {
        printf("%d ", sharedMemory->arr[i]);
    }
    printf("\n");

    shmdt(sharedMemory);
    shmctl(shmId, IPC_RMID, NULL);

    return 0;
}
