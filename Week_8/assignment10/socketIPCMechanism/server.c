#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define FILE_NAME "account.txt"

pthread_mutex_t file_mutex;

void* handle_client(void* arg)
{
    int client_sock = *(int*)arg;
    free(arg);

    int choice;
    int amount;
    char response[256];

    read(client_sock, &choice, sizeof(int));
    read(client_sock, &amount, sizeof(int));

    pthread_mutex_lock(&file_mutex);

    FILE* fp = fopen(FILE_NAME, "r+");
    int balance;
    fscanf(fp, "%d", &balance);

    if(choice == 1)
    {
        if(amount > balance)
            sprintf(response, "Withdrawal Failed! Insufficient Balance: %d", balance);
        else
        {
            balance -= amount;
            rewind(fp);
            fprintf(fp, "%d", balance);
            sprintf(response, "Withdrawal Successful! New Balance: %d", balance);
        }
    }
    else if(choice == 2)
    {
        balance += amount;
        rewind(fp);
        fprintf(fp, "%d", balance);
        sprintf(response, "Deposit Successful! New Balance: %d", balance);
    }
    else if(choice == 3)
    {
        sprintf(response, "Current Balance: %d", balance);
    }
    else
    {
        sprintf(response, "Invalid Operation");
    }

    fclose(fp);
    pthread_mutex_unlock(&file_mutex);

    write(client_sock, response, sizeof(response));
    close(client_sock);

    return NULL;
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    pthread_mutex_init(&file_mutex, NULL);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("ATM Server running on port %d...\n", PORT);

    while(1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

        int* pclient = malloc(sizeof(int));
        *pclient = client_sock;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&file_mutex);
    close(server_sock);
    return 0;
}
