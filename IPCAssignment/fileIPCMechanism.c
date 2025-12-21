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
    FILE *filePointer;

    arrayOfElements = (int*) malloc (numberOfElements * sizeof(int));

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

    filePointer = fopen("arrayDataFile.txt", "w");

    if (filePointer == NULL)
    {
        printf("File Pointer is failed\n");
        return 1;
    }

    for (int index = 0; index < numberOfElements; index++)
    {
        fprintf(filePointer, "%d ", *(arrayOfElements + index));
    }

    fclose(filePointer);
    
    if (fork() == 0)
    {
        filePointer = fopen("arrayDataFile.txt", "r");

        if (filePointer == NULL)
        {
            printf("File Pointer is failed\n");
            return 1;
        }

        for (int index = 0; index < numberOfElements; index++)
        {
            fscanf(filePointer, "%d ", &arrayOfElements[index]);
        }

        fclose(filePointer);

        qsort(arrayOfElements, numberOfElements, sizeof(int), compare);

        filePointer = fopen("arrayDataFile.txt", "w");
        for (int index = 0; index < numberOfElements; index++)
        {
            fprintf(filePointer, "%d ", *(arrayOfElements + index));
        }

        fclose(filePointer);
    }
    else
    {
        wait(NULL);

        filePointer = fopen("arrayDataFile.txt", "r");

        if (filePointer == NULL)
        {
            printf("File Pointer is failed\n");
            return 1;
        }

        for (int index = 0; index < numberOfElements; index++)
        {
            fscanf(filePointer, "%d ", &arrayOfElements[index]);
        }

        fclose(filePointer);
        
        printf("Elements of Array after sorting: \n");

        for (int index = 0; index < numberOfElements; index++)
        {
            printf("%d ", *(arrayOfElements + index));
        }
        printf("\n");
    }

    return 0;
}
