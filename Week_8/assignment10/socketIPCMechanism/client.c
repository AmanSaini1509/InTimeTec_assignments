#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    int choice, amount;
    char response[256];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("\n1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if(choice == 4)
    {
        close(sock);
        exit(0);
    }

    if(choice == 1 || choice == 2)
    {
        printf("Enter amount: ");
        scanf("%d", &amount);
    }
    else
        amount = 0;

    write(sock, &choice, sizeof(int));
    write(sock, &amount, sizeof(int));

    read(sock, response, sizeof(response));
    printf("\nServer Response: %s\n", response);

    close(sock);
    return 0;
}
