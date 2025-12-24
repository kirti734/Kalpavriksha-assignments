#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    int *arrayOfElements;

    arrayOfElements = (int*) malloc (numberOfElements * sizeof(int));

    if (arrayOfElements == NULL)
    {    
        printf("\nArray Of Elements memeory is failed\n");
        return 1
    }

    printf("Enter the elements for the array\n");

    for (int index = 0; index < numberOfElements; index++)
    {
        printf("Enter the element %d : ", (index + 1));
        if (scanf("%d", (arrayOfElements + index)) != 1)
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
        printf("%d ", *(arrayOfElements + index));
    }
    printf("\n");

    int fileDescriptorChildPipe[2];
    int fileDescriptorParentPipe[2];

    pipe(fileDescriptorChildPipe);
    pipe(fileDescriptorParentPipe);

    if (fork() == 0)
    {
        close(fileDescriptorChildPipe[1]);
        close(fileDescriptorParentPipe[0]);

        read(fileDescriptorChildPipe[0], arrayOfElements, numberOfElements * sizeof(int));

        qsort(arrayOfElements, numberOfElements, sizeof(int), compare);

        write(fileDescriptorParentPipe[1], arrayOfElements, numberOfElements * sizeof(int));

        return 0;
    }
    else
    {

        close(fileDescriptorChildPipe[0]);
        close(fileDescriptorParentPipe[1]);

        write(fileDescriptorChildPipe[1], arrayOfElements, numberOfElements * sizeof(int));

        wait(NULL);

        read(fileDescriptorParentPipe[0], arrayOfElements, numberOfElements * sizeof(int));

        printf("Elements of Array After sorting: \n");

        for(int index = 0; index < numberOfElements; index++)
        {
            printf("%d ", *(arrayOfElements + index));
        }
        printf("\n");
    }
}
