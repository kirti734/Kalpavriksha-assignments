#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
	int rollNumber;
	char studentName[100];
	int marks1;
	int marks2;
	int marks3;
};

void recursiveSort(int listofStudents[], int numberofStudents)
{
    if (numberofStudents == 1)
    {
    	return;
	}
	int i;
        
    for (i = 0; i < numberofStudents - 1; i++)
    {
        if (listofStudents[i] > listofStudents[i + 1])
        {
            int temp = listofStudents[i];
            listofStudents[i] = listofStudents[i + 1];
            listofStudents[i + 1] = temp;
        }
    }
    
    recursiveSort(listofStudents, numberofStudents - 1);
}

void performanceAnalyzer(struct User users[] , int numberofStudents)
{
	int i = 0;
	int marksTotal = 0;
	float marksAverage;
	int listofStudents[numberofStudents];
	
	while(i < numberofStudents)
	{
		marksTotal = users[i].marks1 + users[i].marks2 + users[i].marks3;
		marksAverage = marksTotal / numberofStudents;
		listofStudents[i] = users[i].rollNumber;
		
		printf("Roll: %d\n", users[i].rollNumber);
		printf("Name: %s\n", users[i].studentName);
		printf("Total: %d\n", marksTotal);
		printf("Average %f\n", marksAverage);
		
		if (marksAverage >= 85)
		{
		    printf("Grade: A\n");
		    printf("Performance: *****\n");
		}
		else if (marksAverage >= 70)
		{
		    printf("Grade: B\n");
		    printf("Performance: ****\n");
		}
		else if (marksAverage >= 50)
		{
		    printf("Grade: C\n");
		    printf("Performance: ***\n");
		}
		else if (marksAverage >= 35)
		{
		    printf("Grade: D\n");
		    printf("Performance: **\n");
		}
		else
		{
		    printf("Grade: F\n");
		}
		
		printf("\n");
		i++;
	}
	
	i = 0;
	printf("List of Roll Number (via recursion): ");
		
	recursiveSort(listofStudents , numberofStudents);
		
	while (i < numberofStudents)
	{
		printf("%d ", listofStudents[i]);
		i++;
	}
	
}

int main()
{
	int numberofStudents = 0;
	scanf("%d", & numberofStudents);
	
	struct User users[numberofStudents];
	int i;
	
	for (i = 0 ;i < numberofStudents ; i++)
	{
		scanf("%d %s %d %d %d", &users[i].rollNumber, users[i].studentName, &users[i].marks1, &users[i].marks2, &users[i].marks3);
	}
	
	performanceAnalyzer(users , numberofStudents);
}
