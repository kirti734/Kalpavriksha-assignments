#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

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

    int sharedMemoryId;
    key_t key;

    key = ftok("/tmp", 's');
    sharedMemoryId = shmget(key, numberOfElements * sizeof(int), IPC_CREAT | 0666);

    if (sharedMemoryId < 0)
    {
        perror("Shared memory get failed");
        return 1;
    }

    int *sharedMmoryPointer = (int*) shmat (sharedMemoryId, NULL, 0);

    printf("Enter the elements for the array\n");

    for (int index = 0; index < numberOfElements; index++)
    {
        printf("Enter the element %d : ", (index + 1));
        if (scanf("%d", (sharedMmoryPointer + index)) != 1)
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
        printf("%d ", *(sharedMmoryPointer + index));
    }
    printf("\n");

    if (fork() == 0)
    {
        qsort(sharedMmoryPointer, numberOfElements, sizeof(int), compare);

        shmdt(sharedMmoryPointer);

        exit(0);
    }
    else
    {
        wait(NULL);

        printf("Elements of Array After sorting: \n");

        for (int index = 0; index < numberOfElements; index++)
        {
            printf("%d ", *(sharedMmoryPointer + index));
        }
        printf("\n");
        
        shmdt(sharedMmoryPointer);

        shmctl(sharedMemoryId, IPC_RMID, NULL);
    }
    return 0;
}
