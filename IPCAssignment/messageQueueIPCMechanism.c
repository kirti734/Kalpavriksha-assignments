#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX_LENGTH_OF_ARRAY 10000

struct messageBuffer
{
    long messageType;
    int messageText[MAX_LENGTH_OF_ARRAY];
};

int compare(const void *firstElement, const void *secondElement)
{
    return (*(int*)firstElement - *(int*)secondElement);
}

int main()
{
    int numberOfElements = 0;
    printf("Enter the number of elements: ");
    if (scanf("%d", &numberOfElements) != 1)
    {
        printf("Number of elements should be integer\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');

    if (numberOfElements < 0)
    {
        printf("Number of elements should be positive\n");
        return 1;
    }
    
    int messageId;
    key_t key;

    struct messageBuffer message;
    message.messageType = 1;

    key = ftok("/tmp", 'a');
    messageId = msgget(key, IPC_CREAT | 0666);

    printf("Enter the elements for the array\n");

    for (int index = 0; index < numberOfElements; index++)
    {
        printf("Enter the element %d : ", (index + 1));
        if (scanf("%d", &message.messageText[index]) != 1)
        {
            printf("Element should be integer\n");
            while (getchar() != '\n');
            return 1;
        }
        while (getchar() != '\n');
    }

    printf("Elements of Array before sorting: \n");

    for (int index = 0; index < numberOfElements; index++)
    {
        printf("%d ", message.messageText[index]);
    }
    printf("\n");

    msgsnd(messageId, &message, numberOfElements * sizeof(int) , 0);

    if (fork() == 0)
    {
        msgrcv(messageId, &message, numberOfElements * sizeof(int), 1, 0);

        qsort(message.messageText, numberOfElements, sizeof(int), compare);

        message.messageType = 2; 

        msgsnd(messageId, &message, numberOfElements * sizeof(int), 0);

        return 0;
    }
    else
    {
        wait(NULL);

        msgrcv(messageId, &message, numberOfElements * sizeof(int), 2, 0);

        printf("Elements of Array After sorting: \n");

        for (int index = 0; index < numberOfElements; index++)
        {
            printf("%d ", message.messageText[index]);
        }
        printf("\n");

        msgctl(messageId, IPC_RMID, NULL);
    }
    return 0;
}
