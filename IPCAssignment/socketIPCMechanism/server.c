#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define DATABASE_FILE "resource/accountDB.txt"
#define MAX_LENGTH_OF_LINE 1000
#define MAX_NAME_LENGTH 100

pthread_mutex_t fileLock;

struct ClientRequest
{
    int accountNumber;
    int choice;
    int amount;
};

int getLastBalance(int accountNumber)
{
    FILE *filePointer = fopen(DATABASE_FILE, "r");

    if (filePointer == NULL)
    {
        return 1;
    }

    char *tempLine = malloc (MAX_LENGTH_OF_LINE * sizeof(char));

    if (tempLine == NULL)
    {
        printf("Temp Line memory pointer is failed\n");
        return 1;
    }

    int tempBalance = 0;
    int accountBalance = 0;
    int tempAccountNumber = 0;

    while (fgets(tempLine, MAX_LENGTH_OF_LINE, filePointer))
    {
        if (sscanf(tempLine, "%d,%*[^,],%*d,%d", &tempAccountNumber, &tempBalance) == 2)
        {
            if (tempAccountNumber == accountNumber)
            {
                accountBalance = tempBalance;
            }
        }
    }
    
    free(tempLine);
    fclose(filePointer);
    return accountBalance;
}

void logTransaction(int accountNumber, char *status, int amount, int accountBalance)
{
    FILE *filePointer = fopen(DATABASE_FILE, "a");

    if (filePointer == NULL)
    {
        perror("File open failed");
        return;
    }

    fprintf(filePointer, "%d,%s,%d,%d\n", accountNumber, status, amount, accountBalance);

    fclose(filePointer);
}

void* handleClientRequest(void *arg)
{
    int clientSocket = *(int *)arg;
    free(arg);

    struct ClientRequest *request = malloc (sizeof(struct ClientRequest));

    if (request == NULL)
    {
        printf("Mmoery Allocation is failed\n");
        return NULL;
    }

    int currentBalance = 0;

    while (1)
    {
        int bytes = recv(clientSocket, request, sizeof(struct ClientRequest), 0);
        
        if (bytes <= 0 || request->choice == 4)
        {
            break;
        }

        pthread_mutex_lock(&fileLock);

        currentBalance = getLastBalance(request -> accountNumber);

        if (request -> choice == 1)
        {
            if (request -> amount > currentBalance)
            {
                logTransaction(request -> accountNumber, "Withdrawal_failed", request -> amount, currentBalance);

                currentBalance = -1;
            }
            else
            {
                currentBalance -= request -> amount;

                logTransaction(request -> accountNumber, "Withdrawal", request -> amount, currentBalance);
            }
        }
        else if (request -> choice == 2)
        {
            currentBalance += request -> amount;

            logTransaction(request -> accountNumber, "Deposit", request -> amount, currentBalance);
        }
        pthread_mutex_unlock(&fileLock);
        send(clientSocket, &currentBalance, sizeof(int), 0);
    }

    free(request);

    close(clientSocket);
    return NULL;
}

int main()
{
    int serverSocket;
    struct sockaddr_in serverAddress;
    pthread_t threadId;

    pthread_mutex_init(&fileLock, NULL);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;

    serverAddress.sin_port = htons(PORT);

    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    printf("ATM Server running on port %d....\n", PORT);

    while (1)
    {
        int *clientSocket = malloc (sizeof(int));

        if (clientSocket == NULL)
        {
            printf("Mmoery Allocation is failed\n");
            return 1;
        }

        *clientSocket = accept(serverSocket, NULL, NULL);

        pthread_create(&threadId, NULL, handleClientRequest, clientSocket);
        pthread_detach(threadId);
    }

    pthread_mutex_destroy(&fileLock);
    close(serverSocket);

    return 0;
}
