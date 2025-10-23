#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 255

#define MIN_MATRIX_SIZE 2
#define MAX_MATRIX_SIZE 10

void generateMatrix(int sizeofMatrix , int **originalMatrix)
{
	int rowIndex = 0;
	int columnIndex = 0;
	
	for (rowIndex = 0; rowIndex < sizeofMatrix; rowIndex++)
	{
		for (columnIndex = 0; columnIndex < sizeofMatrix; columnIndex++)
		{
			*(*(originalMatrix + rowIndex) + columnIndex) = rand() % (MAX_PIXEL_VALUE - MIN_PIXEL_VALUE + 1) + MIN_PIXEL_VALUE;
		}
	}
}

void swapNumbers(int *firstElement , int *secondElement)
{
	int tempValue = *firstElement;
	*firstElement = *secondElement;
	*secondElement = tempValue;
}

void transposeMatrix(int sizeofMatrix , int **originalMatrix)
{
	int rowIndex = 0;
	int columnIndex = 0;
	
	for (rowIndex = 0; rowIndex < sizeofMatrix; rowIndex++)
	{
		for (columnIndex = (rowIndex + 1); columnIndex < sizeofMatrix; columnIndex++)
		{
			swapNumbers(&*(*(originalMatrix + rowIndex) + columnIndex) , &*(*(originalMatrix + columnIndex) + rowIndex));
		}
	}
}

void reverseRows(int sizeofMatrix , int **originalMatrix)
{
	int rowIndex = 0;
	int leftRowIndex = 0;
	int rightRowIndex = 0;
	
	for (rowIndex = 0; rowIndex < sizeofMatrix; rowIndex++)
	{
		for (leftRowIndex = 0, rightRowIndex = (sizeofMatrix - 1); leftRowIndex < rightRowIndex; leftRowIndex++ , rightRowIndex--)
		{
			swapNumbers(&*(*(originalMatrix + rowIndex) + leftRowIndex) , &*(*(originalMatrix + rowIndex) + rightRowIndex));
		}
	}
}

void rotate90Degree(int sizeofMatrix , int **originalMatrix)
{
	transposeMatrix(sizeofMatrix , originalMatrix);
	reverseRows(sizeofMatrix , originalMatrix);
}

void applySmoothingFilter(int sizeofMatrix , int **originalMatrix)
{
	int *tempSmoothRow = malloc(sizeofMatrix * sizeof(int));
	
	int currentRowIndex = 0;
	int currentColumnIndex = 0;
	
	int neighbourRowIndex = 0;
	int neighbourColumnIndex = 0;
	
	int sumofElements = 0;
	int countofElements = 0;
	
	for (currentColumnIndex = 0; currentColumnIndex < sizeofMatrix; currentColumnIndex++)
	{
		sumofElements = 0;
		countofElements = 0;
			
		for (neighbourRowIndex = 0; neighbourRowIndex <= 1 && neighbourRowIndex <= sizeofMatrix; neighbourRowIndex++)
		{
			for (neighbourColumnIndex = (currentColumnIndex - 1); neighbourColumnIndex <= (currentColumnIndex + 1); neighbourColumnIndex++)
			{
				if (neighbourColumnIndex >= 0 && neighbourColumnIndex < sizeofMatrix)
				{
					sumofElements += *(*(originalMatrix + neighbourRowIndex) + neighbourColumnIndex);
					countofElements++;
				}
			}
		}
		
		*(tempSmoothRow + currentColumnIndex) =  (int)roundf((float)sumofElements / countofElements);	
	}
	
	for (currentRowIndex = 1; currentRowIndex < sizeofMatrix; currentRowIndex++)
	{
		int *tempNextRow = malloc(sizeofMatrix * sizeof(int));
		
		for (currentColumnIndex = 0; currentColumnIndex < sizeofMatrix; currentColumnIndex++)
		{
			sumofElements = 0;
			countofElements = 0;
			
			for (neighbourRowIndex = (currentRowIndex - 1); neighbourRowIndex <= (currentRowIndex + 1); neighbourRowIndex++)
			{
				for (neighbourColumnIndex = (currentColumnIndex - 1); neighbourColumnIndex <= (currentColumnIndex + 1); neighbourColumnIndex++)
				{
					if (neighbourRowIndex >= 0 && neighbourRowIndex < sizeofMatrix && neighbourColumnIndex >= 0 && neighbourColumnIndex < sizeofMatrix)
					{
						sumofElements += *(*(originalMatrix + neighbourRowIndex) + neighbourColumnIndex);
						countofElements++;
					}
				}
			}
			
			*(tempNextRow + currentColumnIndex) =  (int)roundf((float)sumofElements / countofElements);	
		}
		
		for (currentColumnIndex = 0; currentColumnIndex < sizeofMatrix; currentColumnIndex++)
		{
			*(*(originalMatrix + (currentRowIndex - 1)) + currentColumnIndex) = *(tempSmoothRow + currentColumnIndex);
		}
		
		for (currentColumnIndex = 0; currentColumnIndex < sizeofMatrix; currentColumnIndex++)
		{
			*(tempSmoothRow + currentColumnIndex) = *(tempNextRow + currentColumnIndex);
		}
		
		free(tempNextRow);
	}
	
	for (currentColumnIndex = 0; currentColumnIndex < sizeofMatrix; currentColumnIndex++)
	{
		*(*(originalMatrix + (sizeofMatrix - 1)) + currentColumnIndex) = *(tempSmoothRow + currentColumnIndex);
	}
	
	free(tempSmoothRow);
}

void printMatrix(int sizeofMatrix , int **originalMatrix)
{
	int rowIndex = 0;
	int columnIndex = 0;
	
	for (rowIndex = 0; rowIndex < sizeofMatrix; rowIndex++)
	{
		for (columnIndex = 0; columnIndex < sizeofMatrix; columnIndex++)
		{
			printf("%d ", *(*(originalMatrix + rowIndex) + columnIndex));
		}
		printf("\n");
	}
	printf("\n");
}

int main()
{
	int sizeofMatrix;
	
	printf("Enter matrix size (2-10): ");
	
	if (scanf("%d", &sizeofMatrix) != 1)
	{
		printf("Size of Matrix should be a numeric value");
		return 1;
	}
	
	if(sizeofMatrix < 0 || sizeofMatrix < MIN_MATRIX_SIZE || sizeofMatrix > MAX_MATRIX_SIZE)
	{
		printf("Size OF Matrix should be in between 2 and 10");
		return 1;
	}
	
	int index;
	
	int **originalMatrix = malloc(sizeofMatrix * sizeof(int *));
	
	for (int index = 0; index < sizeofMatrix; index++)
	{
		*(originalMatrix + index) = malloc(sizeofMatrix * sizeof(int));
	}
	
	srand(time(NULL));
	
	generateMatrix(sizeofMatrix , originalMatrix);
	
	printf("Original \n");
	printMatrix(sizeofMatrix , originalMatrix);
	
	rotate90Degree(sizeofMatrix , originalMatrix);
	printf("Rotated:\n");
	printMatrix(sizeofMatrix , originalMatrix);
	
	applySmoothingFilter(sizeofMatrix , originalMatrix);
	printf("Final Output\n");
	printMatrix(sizeofMatrix , originalMatrix);
	
	for (int index = 0; index < sizeofMatrix; index++)
	{
		free(*(originalMatrix + index));
	}
	free(originalMatrix);
	
	return 0;
}
