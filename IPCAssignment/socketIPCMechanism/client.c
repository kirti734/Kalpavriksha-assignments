#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

struct ClientRequest
{
    int accountNumber;
    int choice;
    int amount;
};

int main()
{
    int currentBalance;
    int serverSocket;

    struct sockaddr_in serverAddress;

    struct ClientRequest *request = malloc (sizeof(struct ClientRequest));

    if (request == NULL)
    {
        printf("Mmoery Allocation is failed\n");
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;

    serverAddress.sin_port = htons(PORT);

    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    printf("Enter the account number: ");
	  if (scanf("%d", &request -> accountNumber) != 1)
	  {
		    printf("\nAccount number should be numeric\n");
		    while (getchar() != '\n');
		    return 1;
	  }
	  while (getchar() != '\n');

    if (request -> accountNumber <= 0)
    {
        printf("\nAccount number should be positive and non-zero\n");
		    return 1;
	  }

    while(1)
    {
        printf("\n=============================ATM Menu=============================\n");
        printf("\n1. Withdraw\n");
        printf("\n2. Deposit\n");
        printf("\n3. Display Balance\n");
        printf("\n4. Exit\n");

        printf("\nEnter choice: ");
        if (scanf("%d", &request -> choice) != 1)
        {
            printf("\nChoice should be a numeric value\n");
            while (getchar() != '\n');
            continue;
        }	
        while (getchar() != '\n');

        if (request -> choice == 4)
        {
            send(serverSocket, request, sizeof(struct ClientRequest), 0);
            break;
        }

        if (request -> choice == 1 || request -> choice == 2)
        {
            printf("Enter amount: ");
            if (scanf("%d", &request -> amount) != 1)
            {
                printf("Amount should be integer value\n");
                while (getchar() != '\n');
                continue;
            }

            while (getchar() != '\n');

            if (request -> amount < 0)
            {
                printf("\n Amount should be positive\n");
            }
        }
        else
        {
            request -> amount = 0;
        }

        send(serverSocket, request, sizeof(struct ClientRequest), 0);

        recv(serverSocket, &currentBalance, sizeof(int), 0);

        if (request -> choice == 1)
        {
            if (currentBalance == -1)
            {
                printf("Withdrwal Failed: Insufficient Balance\n");
            }
            else
            {
                printf("Withdrawal successful\n");
            }
        }
        else if (request -> choice == 2)
        {
            printf("Deposit successful\n");
        }
        else if (request -> choice == 3)
        {
            printf("Current Balance: %d\n", currentBalance);
        }
    }
    
    free(request);

    close(serverSocket);
    printf("ATM Server is closed\n");
    return 0;
}
