#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 100

struct Message {
    long msg_type;
    int length;
    int arr[MAX_LINE_LENGTH];
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
    int msgId;
    struct Message message;

    FILE *fp = fopen("msgfile", "a");
    fclose(fp);
    
    key = ftok("msgfile", 65);
    msgId = msgget(key, 0666 | IPC_CREAT);

    pid_t processId = fork();

    if(processId > 0)
    {
        printf("Enter number of elements: ");
        if(scanf("%d", &message.length) != 1) {
            printf("Invlaid Input Format\n");
            return 1;
        }
        if(message.length <= 0 || message.length > 100) {
            printf("Invalid input range!\n");
            return 1;
        }
        
        printf("Enter elements:\n");
        for(int i = 0; i < message.length; i++) {
            if(scanf("%d", &message.arr[i]) != 1) {
                printf("Invalid Input Format\n");
                return 1;
            }
        }

        printf("\nArray before sorting:\n");
        for(int i = 0; i < message.length; i++) {
            printf("%d ", message.arr[i]);
        }
        printf("\n");

        message.msg_type = 1;
        msgsnd(msgId, &message, sizeof(message) - sizeof(long), 0);

        msgrcv(msgId, &message, sizeof(message) - sizeof(long), 2, 0);

        printf("\nArray after sorting:\n");
        for(int i = 0; i < message.length; i++)
            printf("%d ", message.arr[i]);
        printf("\n");

        msgctl(msgId, IPC_RMID, NULL);
    }
    else {
        msgrcv(msgId, &message, sizeof(message) - sizeof(long), 1, 0);

        sortArray(message.arr, message.length);

        message.msg_type = 2;
        msgsnd(msgId, &message, sizeof(message) - sizeof(long), 0);

        return 0;
    }

    return 0;
}
